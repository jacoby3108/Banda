


#define BYTE unsigned char
#define PORTBAN  (*((BYTE *) (0x1000)))

#define PA0 0
#define PA1 1
#define PA7 7

#define PIN_DATA (1 << PA0)
#define PIN_CLOCK (1 << PA1)
#define PIN_ENABLE (1 << PA7)


#define LOW 0
#define HIGH 1

#define FALSE 0
#define TRUE 1


#define CLK_LOW  (PIN_CLOCK ^ PIN_CLOCK)
#define CLK_HI PIN_CLOCK



#define START_SENTINEL  0xB  /* P 1101  P=0 (odd parity) */
#define END_SENTINEL  0x1F  /* P 1111  P=1 (odd parity) */
#define FIELD_SEPARATOR  0x0D  /* P 1101  P=0 (odd parity) */


#define  ABA_MASK 0xF8

#define DEBUG TRUE


#define WAITING_FOR_SS  0

#define WAITING_FOR_DATA  1
                              
#define MAXCAP  300

#define OK 1 



/* Lector de banda magnetica definicion de pines */

/*Note:  Enable and data are active low 

*			      |------------------------------|

*			      |	 0v (Gnd)     ->  Negro	     |

data	equ	0	      |  data         ->  Azul	     |

strobe	equ	1	      |	 strobe (CLK) ->  Verde      |

enable	equ	7	      |	 enable       ->  Amarillo   |

*			      |	+5V (Vcc)     ->  Rojo	     |

*			      |------------------------------|


* Nota Enable is active low

*/


BYTE capture [MAXCAP];
BYTE data_buff[100];                
unsigned int pnt=0;

BYTE read_ban(void);

void assemble(unsigned char bit);
void slow(void);
BYTE chk_odd_parity(BYTE val);

void main(void) {
  
  
  while(read_ban());
 
 exit(0);
  
}


BYTE read_ban(void)

{
  
  static tmpbit;
  
  int i=0,j;

  if (!(PORTBAN & PIN_ENABLE))  /* Acitve low !! */
  {
	    while (!(PORTBAN & PIN_ENABLE))  /* While card is sliding */
	    {
	
	
	          if ((PORTBAN & PIN_CLOCK)==CLK_LOW)
	          {
	            
					                                       
		          /*     assemble(PORTBAN & PIN_DATA); */
				      /*   for(j=3;j>0;j--); / settle time */
				      
				      
				    tmpbit=(PORTBAN & PIN_DATA)^PIN_DATA;  /* Isolate bit and invert */
				      
		        #if DEBUG == TRUE
		        
		        capture[i++]=tmpbit;  
		
		        #endif
		        
		        /* assemble(tmpbit); */          /* only for fast processors */ 
		        
		        
		        while ((PORTBAN & PIN_CLOCK)==CLK_LOW); /* Wait for clock rising edge */
		  
	          }
		    
		    
	    }		    
		    
		    slow();
	
  } 
  
  else 
  {
      
      
    
    return(1);
  
  }
	
return(0);

}


void slow(void)

{

int i;

    for(i=0;i<=MAXCAP;i++)
    
    {
        assemble( capture[i] );
      
    }
    
    
    
    


}





void assemble(unsigned char bit)

{
		
static BYTE rx_data=0;      
static BYTE state= WAITING_FOR_SS;
static count=5;

 


        rx_data>>=1;
        
        if(bit)
          rx_data|=0x80;
        else
          rx_data&=0x7F;      


    
    switch (state)  {
    
   
   
  case WAITING_FOR_SS:
    
      if( (rx_data & ABA_MASK) == (START_SENTINEL << 3)) {
        
        printf("SS found \n");
        state=WAITING_FOR_DATA;
        count=5;
        pnt=0;
        
      }
        
       break;              


   case WAITING_FOR_DATA:
   
          putchar((rx_data&0x80)?0x31:0x30);
          
         
          if((--count)==0) { /*if a character was assembled on rx_data store it otherwise continue the assembly process*/
          
                      
              printf("Storing data %d\n",count);
              
             
               /*Check for parity*/
              
               if(chk_odd_parity(rx_data & ABA_MASK)!=OK)
               
                  printf("Parity Error at data %d \n",pnt);
                
                              
                           

                  /*Store assembled data by :
                  
                  1- Isolating P b3 b2 b1 b0 (Most significant part of byte rx_data see ABA MASK definiton ) 
                  2- Shift down until least significand bit of rx_data is reached  
                  3- Set  P=0 
                  4- Add ASCII bias */                           
              
                  data_buff[pnt++]= ((((rx_data&=ABA_MASK)>>3)&0x0F) |0x30);      
                  count=5;  /*Start a new character*/
           
       
       
              /*Check if end of Track*/
          
              if( (rx_data & ABA_MASK) == (END_SENTINEL << 3)) {        /*End Sentinel ??*/
           
                  state=WAITING_FOR_SS;                /*Restart*/
               
                  data_buff[--pnt]=0;                  /*Put null terminator on data string */
              
                  printf("Data Recieved %s \n",data_buff);
              
                  pnt=0;
              
              
                  printf("END Seninel \n");
              
             }    
              
              
          }

         break;
         
     default:
   
           printf("Something wrong !!!! \n");
          
          break;
         
     }

}


/* ============================================================================
 * This funcion asumes that data is stored in the MS part of byte val
 *
 * i.e   p b3 b2 b1 b0 0 0 0 
 *
 *
 * It returns 1 (OK) if parity is odd 0 otherwise
 *
 * Note: even parity can be checked by Strarting pty=1 instead of 0 
 ==============================================================================
 */

BYTE chk_odd_parity(BYTE val) 
{
  
  BYTE walking_one=0x80;
  BYTE pty=0;
  int i; 
  

    for (i=1;i<=5;i++)    /*Count how many ones are in val*/
    {
  
      if(val&walking_one)   /*if a one is present inc pty counter*/
         pty++;

      walking_one>>=1;      /*¨Sift one a place to right */

    }

    return(pty&0x01);  /* Return counter lsb bit (if parrity is odd a 1 (OK) will be returned */

}