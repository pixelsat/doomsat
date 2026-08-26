#!/usr/bin/env bash
# Estimate peak RAM for the doom static lib: static allocations from the
# archive, plus the runtime blocks doom allocates for itself.
set -euo pipefail

LIB=${1:-doom/build/libdoomsat.a}
SIZE=${SIZE:-size}
BUDGET_KIB=${BUDGET_KIB:-368}   # sram1
STACK_KIB=${STACK_KIB:-32}      # guess, see note below

[ -f "$LIB" ] || { echo "no $LIB (run 'just build')" >&2; exit 1; }

# GNU size understands -A; Mach-O size wants -m. Both get normalized to
# "<object> <data|bss|rodata|text> <bytes>" so one awk pass can add them up.
if "$SIZE" -A "$LIB" >/dev/null 2>&1; then
    "$SIZE" -A "$LIB" | awk '
        /\(.*\)/ && /:/ { obj = $0; sub(/^.*\(/, "", obj); sub(/\).*$/, "", obj) }
        $1 == ".data"                     { print obj, "data",   $2 }
        $1 == ".bss" || $1 == ".common"   { print obj, "bss",    $2 }
        $1 ~ /^\.rodata/                  { print obj, "rodata", $2 }
        $1 == ".text"                     { print obj, "text",   $2 }'
else
    "$SIZE" -m "$LIB" | awk '
        /^[^ \t].*\(.*\):$/ { obj = $0; sub(/^.*\(/, "", obj); sub(/\):$/, "", obj) }
        $1 == "Section" {
            s = $3; sub(/\):$/, "", s); n = $4 + 0
            if (s == "__data")                       print obj, "data",   n
            else if (s == "__bss" || s == "__common") print obj, "bss",    n
            else if (s == "__const" || s == "__cstring") print obj, "rodata", n
            else if (s == "__text")                  print obj, "text",   n
        }'
fi > /tmp/doomsize.$$

trap 'rm -f /tmp/doomsize.$$' EXIT

# Runtime blocks doom allocates that never show up in the archive.
zone_mib=$(awk '/^#define DEFAULT_RAM/ { print $3 }' doom/i_system.c)
resx=$(awk '/^#define DOOMGENERIC_RESX/ { print $3 }' doom/doomgeneric.h)
resy=$(awk '/^#define DOOMGENERIC_RESY/ { print $3 }' doom/doomgeneric.h)

echo "top objects by static ram"
awk '$2 == "data" || $2 == "bss" { o[$1] += $3 } END { for (k in o) print o[k], k }' \
    /tmp/doomsize.$$ | sort -rn | head -10 |
    awk '{ printf "  %-24s %8.1f KiB\n", $2, $1 / 1024 }'

awk -v zone_mib="$zone_mib" -v fb=$((resx * resy * 4)) \
    -v stack=$((STACK_KIB * 1024)) -v budget=$((BUDGET_KIB * 1024)) '
    { total[$2] += $3 }
    END {
        zone = zone_mib * 1024 * 1024
        ram = total["data"] + total["bss"] + zone + fb + stack

        printf "\nstatic (in the .a)\n"
        printf "  .data                    %8.1f KiB\n", total["data"] / 1024
        printf "  .bss                     %8.1f KiB\n", total["bss"] / 1024
        printf "\nruntime\n"
        printf "  z_zone heap              %8.1f KiB  (DEFAULT_RAM = %d MiB)\n", zone / 1024, zone_mib
        printf "  DG_ScreenBuffer          %8.1f KiB\n", fb / 1024
        printf "  stack                    %8.1f KiB  (assumed, not measured)\n", stack / 1024
        printf "\n  peak ram                 %8.1f KiB\n", ram / 1024
        printf "  sram1 budget             %8.1f KiB   -> %s by %.1f KiB\n", \
            budget / 1024, (ram <= budget ? "fits" : "OVER"), (ram - budget) / 1024
        printf "\nflash (not ram)\n"
        printf "  .text                    %8.1f KiB\n", total["text"] / 1024
        printf "  .rodata                  %8.1f KiB\n", total["rodata"] / 1024
    }' /tmp/doomsize.$$
