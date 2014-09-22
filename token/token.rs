fn tokenize( data: & str, delimeters: & str ) -> Vec< String >  {
    let mut prev = 0u;
    let mut next = 0u;
    let mut token: Vec< String > = Vec::new();

    for i in data.chars() {
        for j in delimeters.chars() {
            if i == j {
                if next - prev >= 1u {
                    token.push( data.slice_chars( prev, next ).to_string() );
                    prev = next + 1u;
                    break;
                }
                prev = next + 1u;
            }
        }
        next += 1u;
    }
    // add last token
    if next - prev >= 1u {
        token.push( data.slice_chars( prev, next ).to_string() );
    }
    return token;
}

fn main() {
    let data: &'static str = "special! text^= ?123;word @ number#2";
    let del: &'static str = "!^?;@# ";
    let token = tokenize( data, del );

    for i in token.iter() {
        println!( "token: {}", i );
    }
}