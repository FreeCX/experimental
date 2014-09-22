import std.stdio;
import std.string;

void tokenize( ref string[] token, const string data, const string delimeters )
{
    size_t prev, next;

    prev = next = 0;
    foreach ( i ; data ) {
        foreach ( j ; delimeters ) {
            if ( i == j ) {
                if ( next - prev >= 1 ) {
                    token ~= data[prev .. next];
                    prev = next + 1;
                    break;
                }
                prev = next + 1;
            }
        }
        next++;
    }
    // add last token
    if ( next - prev >= 1 ) {
        token ~= data[prev .. next];
    }
}

void main()
{
    string example_str = "special! text^= ?123;word @ number#2";
    string example_del = "!^?;@# ";
    string token[];

    tokenize( token, example_str, example_del );
    foreach ( i ; token ) {
        writeln( "token: ", i );
    }
}
