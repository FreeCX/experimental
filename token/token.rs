trait StrSpec {
    fn slice( &self, begin: usize, end: usize ) -> &str;
}

impl StrSpec for str {
    fn slice( &self, begin: usize, end: usize ) -> &str {
        assert!( begin <= end );
        let mut begin_byte;
        let mut end_byte;

        begin_byte = if begin >= 0 { Some( begin ) } else { None };
        end_byte = if end <= self.len() { Some( end ) } else { Some( self.len() ) };
        match ( begin_byte, end_byte ) {
            ( None, _ ) => panic!( "slice: `begin` is beyond end of string" ),
            ( _, None ) => panic!( "slice: `end` is beyond end of string" ),
            ( Some( a ), Some( b ) ) => unsafe { 
                self.slice_unchecked( a, b )
            }
        }
    }
}

fn tokenize( data: & str, delimeters: & str ) -> Vec< String >  {
    let mut token: Vec< String > = Vec::new();
    let mut prev = 0;
    let mut next = 0;

    for i in data.chars() {
        for j in delimeters.chars() {
            if i == j {
                if next - prev >= 1 {
                    token.push( data.slice( prev, next ).to_string() );
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
        token.push( data.slice( prev, next ).to_string() );
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
