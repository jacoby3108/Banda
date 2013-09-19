	P68H11

*Este archivo contiene las rutinas basicas en assembler para 
*se llamadas desde el 'C'para el Lector de banda magnetica

*Funciones implementadas:

	MODULE	BANDA11

     	PUBLIC	read_band	  Lee una tarjeta 

*======Definiciones para el lector magnetico===========================*

*--------------------------------------
*	ports
*

porta	equ	$1000
banda	equ	porta	 Aca estan Enable,Strobe y data

* Lector de banda magnetica definicion de pines

*			      |------------------------------|
*			      |	 0v (Gnd)     ->  Negro	     |
data	equ	0	      |  data         ->  Azul	     |
strobe	equ	1	      |	 strobe (CLK) ->  Verde      |
enable	equ	7	      |	 enable       ->  Amarillo   |
*			      |	+5V (Vcc)     ->  Rojo	     |
*			      |------------------------------|





*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*
*
*	r a m
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*

	RSEG	UDATA  Reservar lugar para este segmento 
*		       en el archivo del linker	ej UDATA+80


*== ESTA ZONA ES INICIALIZADA CON 00 ==	

ram1	equ	*

pntent	rmb	2		;carga del buffer

*======= COMUNICACION =================

buffer	rmb	16		;buffer de salida.
	rmb	43-16		;43 para la adq. de la banda overly c/buffer
*====== Reservas para el lectror magnetico =======

* De aca para abajo es el buffer de lectura para el magnetico     

bufmag	equ	buffer	   Overlay  con buffer
combuf	equ	buffer+1   comienzo del buffer (+1 pues es compartido el 1ro)
finbuf	equ	*	   final del buffer

count	rmb     1
leido	rmb	1
byte	rmb	1	;armado al leer

dato	rmb    	1
extend	rmb     1

pntbda	rmb	2      Two bytes pointer

finmag  equ	*

endram	equ	*		;fin zona de datos.

*== ACA TERMINA ZONA INICIALIZADA ======



*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*
*
*	r o m
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*

     	RSEG	CODE

*--------------------------------------
*	hardware reset
*--------------------------------------

*--------------------------------------
*	software reset
*--------------------------------------

*============================================================================
* Read_band: Ve si estan pasando una banda
* Si se leyo una banda devuelve un puntero no nulo al buffer 
* que contiene un string ascii (terminado en 0) con la lectura de la banda
* En caso de error o que no se paso tj se devuelve un ptr NULO.
* C prototype -> char *read_band(void)
*=====================================================


read_band equ	*

	jsr	clrvars	    	       Borra la ram desde [RAM1 a ENDRAM]

	ldaa	banda		       Loop de espera del enable
	bmi	sinpul
	jmp	inpbda		       Bajo enable a laburar!!

malban	equ	*
sinpul 	equ	*

	ldd	#0		       retorna Null ptr
	rts



*===== Programa de lector magnetico ===========*

*===========================================*
*
* Inpbda:
*
* Comienza lalectura de la banda magnetica
*
*===========================================*

inpbda	equ	*

	ldx	#bufmag
	stx	pntbda
	ldx	#combuf
	stx	pntent	inicializa banda.

	ldaa	#7+$80
	staa	count		;los 7 primeros los ignoro


*---------------------------------
* aqui comienza la lectura de una banda 
* magnetica.
*---------------------------------

	jsr	rdband		;leo la banda

latoma	ldx	pntbda
	stx	pntent	puntero de salida

	jsr	forban
	bcc	bienb
	jmp	malban


bienb   ldx	#combuf
busc   	inx
	cpx	pntent		;Busco el final para ponerle el teminador.
	bne	busc

sale	clra			'C'string null terminator.
	staa	0,x
	ldd	#combuf		Returns to caller with ptr to combuff
	rts			



*
*--------------------------------------
*
*   lector de banda japones.
*
*--------------------------------------
*
*--------------------------------------
* rutinas de anlisis de lecturas de timer.
*
*	asembl	-entra desde interrupcion
*
*	puntos de entrada.
*
*	wait0	-espera encabezamiento de ceros
*	wait1	-comienza un analisis de lectura
*
*--------------------------------------

asembl	tst	count
	bpl	wait1

wait0	dec	count		;dejo pasar los primeros
	bpl	wait01
return	rts

* aca empiezo con ceros

wait01	inc	count
	bsr	pll
	bcc	return

* aca entro un uno,empieza la lectura de la data

newbyt	ldaa	#5
	staa	count	;4bits mas paridad
	sec
	bra	escero

wait1	bsr	pll

escero	ror	byte
	dec	count
	bne	return

* aca tengo un caracter

	ldaa	byte
	anda	#11111000b

	ldx	pntbda
	cpx	#bufmag		;el primero debe ser start sentinel
	bne	esdata

	cmpa	#01011000b
	beq	esdata
	inc	count		;lo descarto
	bra	return

esdata	cpx	#finbuf
	beq	newbyt		;para no salirme del buffer

	pshx			; incr pntbda
	ldx	pntbda
	inx
	stx	pntbda
	pulx

	asra
	asra
	asra
	anda	#11111b
	staa	,x
	ldaa	#5
	staa	count
	rts

*--------------------------------------
*	pll	controla el clock
*--------------------------------------

pll	eora	#1		; 0 <-> 1
	rora			;data -> Cy
	rts

*--------------------------------------

/*--------------------------------------
*	loop de lectura de banda.
*--------------------------------------
*/


// rdband	equ	*
// pin_data debe ser b0 del puerto

rx_data=0;

if ((PORTBAN & PIN_ENABLE)==SLIDING)
{
	while ((PORTBAN & PIN_ENABLE)==SLIDING)  // While card is sliding
	{
	
		if((PORTBAN & PIN_CLOCK)==LOW)  //Look for falling edge
		{
		
			ldaa	#7+$80;		     // Prepare for reading
				
		
			assemble(PORTBAN & PIN_DATA);
			
			
			while ((PORTBAN & PIN_CLOCK)==LOW) ; // Wait for clock rising edge
		
				
		
		}
		    
		    
	}		    
		    
		    
	
}
	
return(NOBAN);



assemble(unsigned char bit)

{
		
	if (bit_count > (0x80 + 7))   // dejo pasar los primeros
	
			return;
	
	
	rx_data>>=bit;	
	
	
	
	if(bit&0x01)
	
		bit_count=5;	
			
	
	
	else
	
			
			
		
		
		
		
		


]



//pulso	tst	banda
//	bmi	retbnd		 enable=low ?


	

//	ldaa	banda		bajo el clk ?
//	bita	#(1.shl.strobe)
//	bne	pulso

****>

haypul	equ	*

*	brclr	data,banda,leo	 esto es para setear el carry si data = 0
	clrb
****>
	sec
	ldaa	banda		   no afecta al cy
	bita	#(1.shl.data)	    "   "    "   "
	beq 	leo
	clc
****>

leo    	rolb		       
	tba			  	;a=data

*---->
	eora    #1


	jsr	asembl

*	brclr	strobe,banda,$

*****>
w_str	ldaa	banda		 espero que suba el clk
	bita	#(1.shl.strobe)
	beq	w_str
*****>

*******	bra	rdband
	bra	pulso

retbnd 	rts
	

*--------------------------------------
*	forban - analiza la data leida
* del lector de bandas.
*--------------------------------------

forban	ldx	#bufmag

	ldaa	,x
	cmpa	#1011b	;start sent?
	beq	ssent
malbda	sec
	rts

ssent	staa	dato

lopfor	inx
	cpx	#finbuf	;se acabo el buffer?
	beq	malbda

	stx	pntent
	ldaa	,x
	bsr	verpry	;verifico paridad
	bcc	malbda
	eora	dato
	staa	dato
	ldaa	leido
	cmpa	#11111b
	beq	endss

	oraa	#$30
	staa	,x	;data en ascii
	bra	lopfor

endss	ldaa	1,x
	eora	dato	;ahora el lrc
	anda	#1111b
	bne	malbda

clrape	clr	,x
	inx
	cpx	#finbuf
	bne	clrape

	clc
	rts

verpry	staa	leido
	clr	extend	;cuento los unos

loprty	tsta
	bne	ver
	ror	extend	;b0 ->cy
	ldaa	leido
	rts

ver	asla
	bcc	loprty
	inc	extend	;era "1"
	bra	loprty

*---- Fin lector magnetico -----------*

******************************************
******************************************
******************************************
******************************************
******************************************

clrvars	equ	*		        ;Borra la ram desde [RAM1 a ENDRAM]

	ldx	#ram1
lopram	clr	0,x
	inx
	cpx	#endram
	bne	lopram
	rts



*===================================*
*===================================*
*===================================*
*===================================*


	end

