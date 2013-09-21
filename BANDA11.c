


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


#define  ABA_MASK 0xF8

#define DEBUG FALSE



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


BYTE a[500];


BYTE read_ban(void);

void assemble(unsigned char bit);



void main(void) {
  
  
  while(read_ban());
 
 exit(0);
  
}


BYTE read_ban(void)

{
  

  
  int i=0,j;

  if (!(PORTBAN & PIN_ENABLE))  /* Acitve low !! */
  {
	    while (!(PORTBAN & PIN_ENABLE))  /* While card is sliding */
	    {
	
	
	          if ((PORTBAN & PIN_CLOCK)==CLK_LOW)
	          {
	            
					                                       
		          /*     assemble(PORTBAN & PIN_DATA); */
				      /*   for(j=3;j>0;j--); / settle time */
				      
		        #if DEBUG == TRUE
		        
		        a[i++]=(PORTBAN & PIN_DATA)^PIN_DATA;  /* Isolate bit and invert */
		
		        #endif
		        
		        assemble((PORTBAN & PIN_DATA)^PIN_DATA);
		        
		        
		        while ((PORTBAN & PIN_CLOCK)==CLK_LOW); /* Wait for clock rising edge */
		  
	          }
		    
		    
	    }		    
		    
		    
	
  } 
  
  else 
  {
    
    return(1);
  
  }
	
return(0);

}




void assemble(unsigned char bit)

{
		
static BYTE rx_data=0;


   
mal    	rx_data>>=bit;



      if( (rx_data & ABA_MASK) == START_SENTINEL) {
        
        printf("SS found");
        
      }
        
      



}

/*	if (bit_count > (0x80 + 7))   // dejo pasar los primeros
	
			return;
	
*/	
	
	
	
	
		
	
/*	
	switch (state)


	case (WAIT_FOR_ONE):

	
		if(bit&0x01)
		{
			bit_count=5;	
			state=START_READING;		
	
		}
		else
	 
		return;
	
	
	break;

	case(START_READING):
	
		if(bit_count)
		{
			rx_data>>=bit;
			bit_count--;
		}	
		else 
		{
			if(rx_data==SS)
			{
				*p2buffer++=rx_data&DATA_MASK;	
				bit_count=5;
			{
			else 
			{

							

		}
	

	break;		

		
	default:
	break;
			
*/
		
