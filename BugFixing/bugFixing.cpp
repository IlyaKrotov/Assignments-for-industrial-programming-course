/**
* @brief
*		Find errors and decrease probability of getting errors of the same kind in the future
*		This piece of code won't compile and it doesn't describe an entire algorithm: just part of some page storage
*
* @author
*		AnnaM
*/

#include <Windows.h>
#include <stdio.h>
#include <assert.h>

enum PAGE_COLOR
{
    PG_COLOR_GREEN, /* page may be released without high overhead */                /* Added, numbering starts from 0 */
	PG_COLOR_YELLOW, /* nice to have */
    PG_COLOR_RED,	/* page is actively used */
    PG_NUMBER_OF_COLORS                                                             /* Added, number of elements in enum */
};


/**
 * UINT Key of a page in hash-table (prepared from color and address)
 */
union PageKey
{
	struct
	{
        UINT	cColor: 8;                                                          /* Added, CHAR -> UINT */
		UINT	cAddr: 24;
	};

	UINT	uKey;
};


/* Prepare from 2 chars the key of the same configuration as in PageKey */
#define CALC_PAGE_KEY( Addr, Color )	(  (Color) + ( (Addr) << 8 ) )             /* Added, Priority of opperations */


/**
 * Descriptor of a single guest physical page
 */
struct PageDesc
{
	PageKey			uKey;	

	/* list support */
	PageDesc		*next, *prev;
};

#define PAGE_INIT( Desc, Addr, Color )              \
    do{                                             \
        (Desc).uKey = CALC_PAGE_KEY( Addr, Color ); \
        (Desc).next = (Desc).prev = NULL;           \
    } while(0)                                                                    /* Added, 'do{} while(0)' */

/* storage for pages of all colors */
static PageDesc* PageStrg[ PG_NUMBER_OF_COLORS ];

void PageStrgInit()
{
    memset( PageStrg, 0, sizeof(*PageStrg) * PG_NUMBER_OF_COLORS);                /* Changed, don't need '&' and memory for array */
}

PageDesc* PageFind( void* ptr, unsigned char color )
{
    assert(color < PG_NUMBER_OF_COLORS);/* Added, assert for color */
    
    for( PageDesc* Pg = PageStrg[color]; Pg; Pg = Pg->next )                      /* Added, don't need ';', [color]: char -> unsigned char(scalability) and color < number of colors */
        if( Pg->uKey == CALC_PAGE_KEY((UINT)ptr,color) )                          /* Added, if 64-bit CALC_PAGE_KEY will be worked bad */
           return Pg;                                                                                                                                     
    return NULL;
}

PageDesc* PageReclaim( UINT cnt )
{
	UINT color = 0;
    PageDesc* Pg = PageStrg[0];                                                   /* Added, needs initialize */
	while( cnt )
    {
        if( Pg == NULL )                                                          /* Changed order of code */
        {                                                                         /* overflow[] */
            color++;
            Pg = PageStrg[ color ];
        }

		Pg = Pg->next;
		PageRemove( PageStrg[ color ] );
		cnt--;		
	}    
    return Pg;                                                                    /* Added,  There is not 'return'. This func returns pointer on first not NULL element */
}
            
PageDesc* PageInit( void* ptr, UINT color )
{
    try
    {
        PageDesc* pg = new PageDesc;
        if( pg )
            PAGE_INIT(*pg, ptr, color);                                           /* Added, is enough ('&'->'*') and ; (fixed in define) */
        else
            printf("Allocation has failed\n");
        return pg;
    }
    catch(bad_alloc)                                                              /* Added, exception */
    {
        cerr << "Bad allocation" << endl;
        return NULL;
    }

    return NULL;

}

/**
 * Print all mapped pages
 */
void PageDump()
{
	UINT color = 0;
	#define PG_COLOR_NAME(clr) #clr
	char* PgColorName[] = 
	{
        PG_COLOR_NAME(PG_COLOR_GREEN),                                          /* Changed: wrong order */
		PG_COLOR_NAME(PG_COLOR_YELLOW),		
        PG_COLOR_NAME(PG_COLOR_RED)
	};

	while( color <= PG_COLOR_RED )
	{
        printf("PgStrg[(%s) %u] ********** \n", PgColorName[color], color );    /* Changed: wrong order */
        for( PageDesc* Pg = PageStrg[color++]; Pg != NULL; Pg = Pg->next )      /* Changed: ++ should be postfix */
		{
            if( Pg->uAddr == NULL )                                             /* Changed: == */
                continue;

			printf("Pg :Key = 0x%x, addr %p\n", Pg->uKey, Pg->uAddr );
		}
	}
	#undef PG_COLOR_NAME
}
