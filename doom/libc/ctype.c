int
isspace (int c)
{
    return c == ' ' || ('\t' <= c && c <= '\r');
}