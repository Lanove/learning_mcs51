                                      1 ;--------------------------------------------------------
                                      2 ; File Created by SDCC : free open source ISO C Compiler 
                                      3 ; Version 4.4.0 #14620 (Linux)
                                      4 ;--------------------------------------------------------
                                      5 	.module blink
                                      6 	.optsdcc -mmcs51 --model-small
                                      7 	
                                      8 ;--------------------------------------------------------
                                      9 ; Public variables in this module
                                     10 ;--------------------------------------------------------
                                     11 	.globl _main
                                     12 	.globl _CY
                                     13 	.globl _AC
                                     14 	.globl _F0
                                     15 	.globl _RS1
                                     16 	.globl _RS0
                                     17 	.globl _OV
                                     18 	.globl _F1
                                     19 	.globl _P
                                     20 	.globl _PS
                                     21 	.globl _PT1
                                     22 	.globl _PX1
                                     23 	.globl _PT0
                                     24 	.globl _PX0
                                     25 	.globl _RD
                                     26 	.globl _WR
                                     27 	.globl _T1
                                     28 	.globl _T0
                                     29 	.globl _INT1
                                     30 	.globl _INT0
                                     31 	.globl _TXD
                                     32 	.globl _RXD
                                     33 	.globl _P3_7
                                     34 	.globl _P3_6
                                     35 	.globl _P3_5
                                     36 	.globl _P3_4
                                     37 	.globl _P3_3
                                     38 	.globl _P3_2
                                     39 	.globl _P3_1
                                     40 	.globl _P3_0
                                     41 	.globl _EA
                                     42 	.globl _ES
                                     43 	.globl _ET1
                                     44 	.globl _EX1
                                     45 	.globl _ET0
                                     46 	.globl _EX0
                                     47 	.globl _P2_7
                                     48 	.globl _P2_6
                                     49 	.globl _P2_5
                                     50 	.globl _P2_4
                                     51 	.globl _P2_3
                                     52 	.globl _P2_2
                                     53 	.globl _P2_1
                                     54 	.globl _P2_0
                                     55 	.globl _SM0
                                     56 	.globl _SM1
                                     57 	.globl _SM2
                                     58 	.globl _REN
                                     59 	.globl _TB8
                                     60 	.globl _RB8
                                     61 	.globl _TI
                                     62 	.globl _RI
                                     63 	.globl _P1_7
                                     64 	.globl _P1_6
                                     65 	.globl _P1_5
                                     66 	.globl _P1_4
                                     67 	.globl _P1_3
                                     68 	.globl _P1_2
                                     69 	.globl _P1_1
                                     70 	.globl _P1_0
                                     71 	.globl _TF1
                                     72 	.globl _TR1
                                     73 	.globl _TF0
                                     74 	.globl _TR0
                                     75 	.globl _IE1
                                     76 	.globl _IT1
                                     77 	.globl _IE0
                                     78 	.globl _IT0
                                     79 	.globl _P0_7
                                     80 	.globl _P0_6
                                     81 	.globl _P0_5
                                     82 	.globl _P0_4
                                     83 	.globl _P0_3
                                     84 	.globl _P0_2
                                     85 	.globl _P0_1
                                     86 	.globl _P0_0
                                     87 	.globl _B
                                     88 	.globl _ACC
                                     89 	.globl _PSW
                                     90 	.globl _IP
                                     91 	.globl _P3
                                     92 	.globl _IE
                                     93 	.globl _P2
                                     94 	.globl _SBUF
                                     95 	.globl _SCON
                                     96 	.globl _P1
                                     97 	.globl _TH1
                                     98 	.globl _TH0
                                     99 	.globl _TL1
                                    100 	.globl _TL0
                                    101 	.globl _TMOD
                                    102 	.globl _TCON
                                    103 	.globl _PCON
                                    104 	.globl _DPH
                                    105 	.globl _DPL
                                    106 	.globl _SP
                                    107 	.globl _P0
                                    108 	.globl _delay
                                    109 	.globl _fast_delay
                                    110 ;--------------------------------------------------------
                                    111 ; special function registers
                                    112 ;--------------------------------------------------------
                                    113 	.area RSEG    (ABS,DATA)
      000000                        114 	.org 0x0000
                           000080   115 _P0	=	0x0080
                           000081   116 _SP	=	0x0081
                           000082   117 _DPL	=	0x0082
                           000083   118 _DPH	=	0x0083
                           000087   119 _PCON	=	0x0087
                           000088   120 _TCON	=	0x0088
                           000089   121 _TMOD	=	0x0089
                           00008A   122 _TL0	=	0x008a
                           00008B   123 _TL1	=	0x008b
                           00008C   124 _TH0	=	0x008c
                           00008D   125 _TH1	=	0x008d
                           000090   126 _P1	=	0x0090
                           000098   127 _SCON	=	0x0098
                           000099   128 _SBUF	=	0x0099
                           0000A0   129 _P2	=	0x00a0
                           0000A8   130 _IE	=	0x00a8
                           0000B0   131 _P3	=	0x00b0
                           0000B8   132 _IP	=	0x00b8
                           0000D0   133 _PSW	=	0x00d0
                           0000E0   134 _ACC	=	0x00e0
                           0000F0   135 _B	=	0x00f0
                                    136 ;--------------------------------------------------------
                                    137 ; special function bits
                                    138 ;--------------------------------------------------------
                                    139 	.area RSEG    (ABS,DATA)
      000000                        140 	.org 0x0000
                           000080   141 _P0_0	=	0x0080
                           000081   142 _P0_1	=	0x0081
                           000082   143 _P0_2	=	0x0082
                           000083   144 _P0_3	=	0x0083
                           000084   145 _P0_4	=	0x0084
                           000085   146 _P0_5	=	0x0085
                           000086   147 _P0_6	=	0x0086
                           000087   148 _P0_7	=	0x0087
                           000088   149 _IT0	=	0x0088
                           000089   150 _IE0	=	0x0089
                           00008A   151 _IT1	=	0x008a
                           00008B   152 _IE1	=	0x008b
                           00008C   153 _TR0	=	0x008c
                           00008D   154 _TF0	=	0x008d
                           00008E   155 _TR1	=	0x008e
                           00008F   156 _TF1	=	0x008f
                           000090   157 _P1_0	=	0x0090
                           000091   158 _P1_1	=	0x0091
                           000092   159 _P1_2	=	0x0092
                           000093   160 _P1_3	=	0x0093
                           000094   161 _P1_4	=	0x0094
                           000095   162 _P1_5	=	0x0095
                           000096   163 _P1_6	=	0x0096
                           000097   164 _P1_7	=	0x0097
                           000098   165 _RI	=	0x0098
                           000099   166 _TI	=	0x0099
                           00009A   167 _RB8	=	0x009a
                           00009B   168 _TB8	=	0x009b
                           00009C   169 _REN	=	0x009c
                           00009D   170 _SM2	=	0x009d
                           00009E   171 _SM1	=	0x009e
                           00009F   172 _SM0	=	0x009f
                           0000A0   173 _P2_0	=	0x00a0
                           0000A1   174 _P2_1	=	0x00a1
                           0000A2   175 _P2_2	=	0x00a2
                           0000A3   176 _P2_3	=	0x00a3
                           0000A4   177 _P2_4	=	0x00a4
                           0000A5   178 _P2_5	=	0x00a5
                           0000A6   179 _P2_6	=	0x00a6
                           0000A7   180 _P2_7	=	0x00a7
                           0000A8   181 _EX0	=	0x00a8
                           0000A9   182 _ET0	=	0x00a9
                           0000AA   183 _EX1	=	0x00aa
                           0000AB   184 _ET1	=	0x00ab
                           0000AC   185 _ES	=	0x00ac
                           0000AF   186 _EA	=	0x00af
                           0000B0   187 _P3_0	=	0x00b0
                           0000B1   188 _P3_1	=	0x00b1
                           0000B2   189 _P3_2	=	0x00b2
                           0000B3   190 _P3_3	=	0x00b3
                           0000B4   191 _P3_4	=	0x00b4
                           0000B5   192 _P3_5	=	0x00b5
                           0000B6   193 _P3_6	=	0x00b6
                           0000B7   194 _P3_7	=	0x00b7
                           0000B0   195 _RXD	=	0x00b0
                           0000B1   196 _TXD	=	0x00b1
                           0000B2   197 _INT0	=	0x00b2
                           0000B3   198 _INT1	=	0x00b3
                           0000B4   199 _T0	=	0x00b4
                           0000B5   200 _T1	=	0x00b5
                           0000B6   201 _WR	=	0x00b6
                           0000B7   202 _RD	=	0x00b7
                           0000B8   203 _PX0	=	0x00b8
                           0000B9   204 _PT0	=	0x00b9
                           0000BA   205 _PX1	=	0x00ba
                           0000BB   206 _PT1	=	0x00bb
                           0000BC   207 _PS	=	0x00bc
                           0000D0   208 _P	=	0x00d0
                           0000D1   209 _F1	=	0x00d1
                           0000D2   210 _OV	=	0x00d2
                           0000D3   211 _RS0	=	0x00d3
                           0000D4   212 _RS1	=	0x00d4
                           0000D5   213 _F0	=	0x00d5
                           0000D6   214 _AC	=	0x00d6
                           0000D7   215 _CY	=	0x00d7
                                    216 ;--------------------------------------------------------
                                    217 ; overlayable register banks
                                    218 ;--------------------------------------------------------
                                    219 	.area REG_BANK_0	(REL,OVR,DATA)
      000000                        220 	.ds 8
                                    221 ;--------------------------------------------------------
                                    222 ; internal ram data
                                    223 ;--------------------------------------------------------
                                    224 	.area DSEG    (DATA)
                                    225 ;--------------------------------------------------------
                                    226 ; overlayable items in internal ram
                                    227 ;--------------------------------------------------------
                                    228 	.area	OSEG    (OVR,DATA)
                                    229 	.area	OSEG    (OVR,DATA)
                                    230 ;--------------------------------------------------------
                                    231 ; Stack segment in internal ram
                                    232 ;--------------------------------------------------------
                                    233 	.area SSEG
      000008                        234 __start__stack:
      000008                        235 	.ds	1
                                    236 
                                    237 ;--------------------------------------------------------
                                    238 ; indirectly addressable internal ram data
                                    239 ;--------------------------------------------------------
                                    240 	.area ISEG    (DATA)
                                    241 ;--------------------------------------------------------
                                    242 ; absolute internal ram data
                                    243 ;--------------------------------------------------------
                                    244 	.area IABS    (ABS,DATA)
                                    245 	.area IABS    (ABS,DATA)
                                    246 ;--------------------------------------------------------
                                    247 ; bit data
                                    248 ;--------------------------------------------------------
                                    249 	.area BSEG    (BIT)
                                    250 ;--------------------------------------------------------
                                    251 ; paged external ram data
                                    252 ;--------------------------------------------------------
                                    253 	.area PSEG    (PAG,XDATA)
                                    254 ;--------------------------------------------------------
                                    255 ; uninitialized external ram data
                                    256 ;--------------------------------------------------------
                                    257 	.area XSEG    (XDATA)
                                    258 ;--------------------------------------------------------
                                    259 ; absolute external ram data
                                    260 ;--------------------------------------------------------
                                    261 	.area XABS    (ABS,XDATA)
                                    262 ;--------------------------------------------------------
                                    263 ; initialized external ram data
                                    264 ;--------------------------------------------------------
                                    265 	.area XISEG   (XDATA)
                                    266 	.area HOME    (CODE)
                                    267 	.area GSINIT0 (CODE)
                                    268 	.area GSINIT1 (CODE)
                                    269 	.area GSINIT2 (CODE)
                                    270 	.area GSINIT3 (CODE)
                                    271 	.area GSINIT4 (CODE)
                                    272 	.area GSINIT5 (CODE)
                                    273 	.area GSINIT  (CODE)
                                    274 	.area GSFINAL (CODE)
                                    275 	.area CSEG    (CODE)
                                    276 ;--------------------------------------------------------
                                    277 ; interrupt vector
                                    278 ;--------------------------------------------------------
                                    279 	.area HOME    (CODE)
      000000                        280 __interrupt_vect:
      000000 02 00 06         [24]  281 	ljmp	__sdcc_gsinit_startup
                                    282 ;--------------------------------------------------------
                                    283 ; global & static initialisations
                                    284 ;--------------------------------------------------------
                                    285 	.area HOME    (CODE)
                                    286 	.area GSINIT  (CODE)
                                    287 	.area GSFINAL (CODE)
                                    288 	.area GSINIT  (CODE)
                                    289 	.globl __sdcc_gsinit_startup
                                    290 	.globl __sdcc_program_startup
                                    291 	.globl __start__stack
                                    292 	.globl __mcs51_genXINIT
                                    293 	.globl __mcs51_genXRAMCLEAR
                                    294 	.globl __mcs51_genRAMCLEAR
                                    295 	.area GSFINAL (CODE)
      00005F 02 00 03         [24]  296 	ljmp	__sdcc_program_startup
                                    297 ;--------------------------------------------------------
                                    298 ; Home
                                    299 ;--------------------------------------------------------
                                    300 	.area HOME    (CODE)
                                    301 	.area HOME    (CODE)
      000003                        302 __sdcc_program_startup:
      000003 02 00 62         [24]  303 	ljmp	_main
                                    304 ;	return from main will return to caller
                                    305 ;--------------------------------------------------------
                                    306 ; code
                                    307 ;--------------------------------------------------------
                                    308 	.area CSEG    (CODE)
                                    309 ;------------------------------------------------------------
                                    310 ;Allocation info for local variables in function 'main'
                                    311 ;------------------------------------------------------------
                                    312 ;	blink.c:6: void main(void)
                                    313 ;	-----------------------------------------
                                    314 ;	 function main
                                    315 ;	-----------------------------------------
      000062                        316 _main:
                           000007   317 	ar7 = 0x07
                           000006   318 	ar6 = 0x06
                           000005   319 	ar5 = 0x05
                           000004   320 	ar4 = 0x04
                           000003   321 	ar3 = 0x03
                           000002   322 	ar2 = 0x02
                           000001   323 	ar1 = 0x01
                           000000   324 	ar0 = 0x00
                                    325 ;	blink.c:8: while (1)
      000062                        326 00102$:
                                    327 ;	blink.c:10: P0 = 0xFF; // Turn ON all LED's connected to Port1
      000062 75 80 FF         [24]  328 	mov	_P0,#0xff
                                    329 ;	blink.c:11: P1 = 0xFF; // Turn ON all LED's connected to Port1
      000065 75 90 FF         [24]  330 	mov	_P1,#0xff
                                    331 ;	blink.c:12: P2 = 0xFF; // Turn ON all LED's connected to Port1
      000068 75 A0 FF         [24]  332 	mov	_P2,#0xff
                                    333 ;	blink.c:13: P3 = 0xFF; // Turn ON all LED's connected to Port1
      00006B 75 B0 FF         [24]  334 	mov	_P3,#0xff
                                    335 ;	blink.c:14: delay();
      00006E 12 00 82         [24]  336 	lcall	_delay
                                    337 ;	blink.c:15: P0 = 0x00; // Turn OFF all LED's connected to Port1
      000071 75 80 00         [24]  338 	mov	_P0,#0x00
                                    339 ;	blink.c:16: P1 = 0x00; // Turn OFF all LED's connected to Port1
      000074 75 90 00         [24]  340 	mov	_P1,#0x00
                                    341 ;	blink.c:17: P2 = 0x00; // Turn OFF all LED's connected to Port1
      000077 75 A0 00         [24]  342 	mov	_P2,#0x00
                                    343 ;	blink.c:18: P3 = 0x00; // Turn OFF all LED's connected to Port1
      00007A 75 B0 00         [24]  344 	mov	_P3,#0x00
                                    345 ;	blink.c:19: delay();
      00007D 12 00 82         [24]  346 	lcall	_delay
                                    347 ;	blink.c:21: }
      000080 80 E0            [24]  348 	sjmp	00102$
                                    349 ;------------------------------------------------------------
                                    350 ;Allocation info for local variables in function 'delay'
                                    351 ;------------------------------------------------------------
                                    352 ;i                         Allocated to registers r6 r7 
                                    353 ;j                         Allocated to registers r4 r5 
                                    354 ;------------------------------------------------------------
                                    355 ;	blink.c:23: void delay(void)
                                    356 ;	-----------------------------------------
                                    357 ;	 function delay
                                    358 ;	-----------------------------------------
      000082                        359 _delay:
                                    360 ;	blink.c:26: for (i = 0; i < 0xff; i++)
      000082 7E 00            [12]  361 	mov	r6,#0x00
      000084 7F 00            [12]  362 	mov	r7,#0x00
                                    363 ;	blink.c:27: for (j = 0; j < 0xff; j++);
      000086                        364 00110$:
      000086 7C FF            [12]  365 	mov	r4,#0xff
      000088 7D 00            [12]  366 	mov	r5,#0x00
      00008A                        367 00105$:
      00008A 1C               [12]  368 	dec	r4
      00008B BC FF 01         [24]  369 	cjne	r4,#0xff,00130$
      00008E 1D               [12]  370 	dec	r5
      00008F                        371 00130$:
      00008F EC               [12]  372 	mov	a,r4
      000090 4D               [12]  373 	orl	a,r5
      000091 70 F7            [24]  374 	jnz	00105$
                                    375 ;	blink.c:26: for (i = 0; i < 0xff; i++)
      000093 0E               [12]  376 	inc	r6
      000094 BE 00 01         [24]  377 	cjne	r6,#0x00,00132$
      000097 0F               [12]  378 	inc	r7
      000098                        379 00132$:
      000098 C3               [12]  380 	clr	c
      000099 EE               [12]  381 	mov	a,r6
      00009A 94 FF            [12]  382 	subb	a,#0xff
      00009C EF               [12]  383 	mov	a,r7
      00009D 64 80            [12]  384 	xrl	a,#0x80
      00009F 94 80            [12]  385 	subb	a,#0x80
      0000A1 40 E3            [24]  386 	jc	00110$
                                    387 ;	blink.c:28: }
      0000A3 22               [24]  388 	ret
                                    389 ;------------------------------------------------------------
                                    390 ;Allocation info for local variables in function 'fast_delay'
                                    391 ;------------------------------------------------------------
                                    392 ;i                         Allocated to registers r6 r7 
                                    393 ;j                         Allocated to registers r4 r5 
                                    394 ;------------------------------------------------------------
                                    395 ;	blink.c:30: void fast_delay(void)
                                    396 ;	-----------------------------------------
                                    397 ;	 function fast_delay
                                    398 ;	-----------------------------------------
      0000A4                        399 _fast_delay:
                                    400 ;	blink.c:33: for (i = 0; i < 0xff; i++)
      0000A4 7E 00            [12]  401 	mov	r6,#0x00
      0000A6 7F 00            [12]  402 	mov	r7,#0x00
                                    403 ;	blink.c:34: for (j = 0; j < 0x60; j++)
      0000A8                        404 00110$:
      0000A8 7C 60            [12]  405 	mov	r4,#0x60
      0000AA 7D 00            [12]  406 	mov	r5,#0x00
      0000AC                        407 00105$:
      0000AC 1C               [12]  408 	dec	r4
      0000AD BC FF 01         [24]  409 	cjne	r4,#0xff,00130$
      0000B0 1D               [12]  410 	dec	r5
      0000B1                        411 00130$:
      0000B1 EC               [12]  412 	mov	a,r4
      0000B2 4D               [12]  413 	orl	a,r5
      0000B3 70 F7            [24]  414 	jnz	00105$
                                    415 ;	blink.c:33: for (i = 0; i < 0xff; i++)
      0000B5 0E               [12]  416 	inc	r6
      0000B6 BE 00 01         [24]  417 	cjne	r6,#0x00,00132$
      0000B9 0F               [12]  418 	inc	r7
      0000BA                        419 00132$:
      0000BA C3               [12]  420 	clr	c
      0000BB EE               [12]  421 	mov	a,r6
      0000BC 94 FF            [12]  422 	subb	a,#0xff
      0000BE EF               [12]  423 	mov	a,r7
      0000BF 64 80            [12]  424 	xrl	a,#0x80
      0000C1 94 80            [12]  425 	subb	a,#0x80
      0000C3 40 E3            [24]  426 	jc	00110$
                                    427 ;	blink.c:36: }
      0000C5 22               [24]  428 	ret
                                    429 	.area CSEG    (CODE)
                                    430 	.area CONST   (CODE)
                                    431 	.area XINIT   (CODE)
                                    432 	.area CABS    (ABS,CODE)
