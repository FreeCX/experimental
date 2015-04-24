#![feature(collections)]

fn tokenize( data: & str, delimeters: & str ) -> Vec< String >  {
    let mut token: Vec< String > = Vec::new();
    let mut prev = 0;
    let mut next = 0;

    for i in data.chars() {
        for j in delimeters.chars() {
            if i == j {
                if next - prev >= 1 {
                    token.push( data.slice_chars( prev, next ).to_string() );
                    prev = next + 1;
                    break;
                }
                prev = next + 1;
            }
        }
        next += 1;
    }
    // add last token
    if next - prev >= 1 {
        token.push( data.slice_chars( prev, next ).to_string() );
    }
    return token;
}

fn main() {
    let data: &'static str = "special! text^= ?123;word @ number#2";
    let del: &'static str = "!^?;@# ";
    let token = tokenize( data, del );

    for i in token {
        println!( "token: {}", i );
    }
}
