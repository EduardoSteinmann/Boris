#include "test.boris.h"


int    main()    
{
    const auto s  = (StrLit){ "Hello World" };
    printf("%s", s);
    const auto a  = 4;
    const auto f  = 6;
    if ( true )
    {
        printf("Inside if statement");
    }
    else if ( false )
    {
        printf("Testing else if");
    }
    else
    {
        printf("Else Statement");
    }

    for ( size_t i = 0; i < 1; i++ )
    {
        auto d      = 5;
        printf("Inside for loop");
    }

    printf("Hello World");
    return 0;
}
