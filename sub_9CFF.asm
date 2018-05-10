; =============== S U B R O U T I N E =======================================
; https://tetris.wiki/Dr._Mario#Virus_Generation


sub_9CFF:                               ; CODE XREF: sub_9C91+13p sub_9C91+2Bp
                LDA     REM_VIRUSES     ; Remaining viruses to generate into the bottle.
                BNE     loc_9D06
                JMP     locret_9E4B
; ---------------------------------------------------------------------------

loc_9D06:                               ; CODE XREF: sub_9CFF+2j sub_9CFF+1Bj
                LDX     #byte_17
                LDY     #2
                JSR     sub_B771        ; Y contains the number of bytes to rotate right one bit. When Y is 0 it's as if Y were 256.
                                        ; X contains an offset from memory location 0 to start operating on.
                                        ; Memory locations 0+X to 0+X+Y-1 contain data to operate on.
                                        ;
                                        ; The first word of memory is treated specially; firstly, the byte at 0+X will be ANDed with 2
                                        ; before operations procede, then if (m(0+X) & 2) ^ (m(1+X) & 2) == 0, where m(?) is
                                        ; "memory at location ?", the right rotating will begin with the carry flag cleared;
                                        ; if that EOR expression isn't zero, the right rotating will begin with the carry flag set.
                LDA     byte_17         ; Get a random byte.
                AND     #$F             ; Mask off upper nibble.
                STA     byte_47         ; Store random number into byte_47.
                LDX     VIRUS_LEVEL     ; Get current virus level.
                LDA     byte_A3DE,X     ; Get byte from table indexed by the current virus level.
                CMP     byte_47         ; A < byte_47? (unsigned)
                BCC     loc_9D06        ; Branch if A < byte_47. (unsigned)
                LDA     byte_47         ; Get random number from byte_47.
                TAX
                LDA     byte_A474,X     ; Get into A a number from the data table indexed by a random number.
                STA     byte_47         ; Store number from the data table.
                LDA     byte_18         ; Get into A a random number.
                AND     #7              ; Mask off upper 5 bits.
                CLC
                ADC     byte_47         ; Add to data table value a number in range [0,7].
                STA     byte_57         ; Store name table pointer location.
                STA     byte_49         ; Store location on-screen to check.
                LDA     REM_VIRUSES     ; Remaining viruses to generate into the bottle.
                AND     #3              ; Mask off upper 6 bits.
                STA     byte_48
                CMP     #3
                BNE     loc_9D4C        ; Branch if number isn't 3. (it's in range [0,2])
                LDX     #$17
                LDY     #2
                JSR     sub_B771        ; Rotate right one bit the bytes at locations $17 and $18.
                LDA     byte_18
                AND     #$F
                TAX
                LDA     byte_A7ED,X
                AND     #3
                STA     byte_48

loc_9D4C:                               ; CODE XREF: sub_9CFF+38j sub_9CFF+5Dj
                LDY     #0
                LDA     (byte_57),Y     ; Get a byte of the bottle.
                CMP     #$FF
                BEQ     loc_9D61        ; If the byte is empty ($FF), branch.

loc_9D54:                               ; CODE XREF: sub_9CFF+E6j
                INC     byte_49         ; Advance screen location to check.
                LDA     byte_49         ; Get screen location to check.
                STA     byte_57         ; Store location on screen to check.
                CMP     #$80            ; A < $80?
                BCC     loc_9D4C        ; Branch if A is less than $80 (unsigned).
                JMP     locret_9E4B     ; Else, return.
; ---------------------------------------------------------------------------

loc_9D61:                               ; CODE XREF: sub_9CFF+53j
                LDA     byte_48         ; Get integer in range [0,2].
                STA     byte_47         ; Save integer in range [0,2].
                LDX     SPEED           ; Get current speed.
                LDA     byte_A390,X     ; Get number from table indexed by speed.
                STA     byte_4A         ; Store 16.
                LDX     SPEED           ; Get current speed.
                LDA     byte_A393,X     ; Get number from table indexed by speed.
                STA     byte_4B         ; Store 2.
                LDA     #0
                STA     byte_4C         ; Clear byte $4C.
                LDA     #0
                STA     byte_57         ; Clear byte $57.
                LDA     byte_49         ; Get screen location.
                SEC
                SBC     byte_4A         ; Subtract 16 from screen location. (move up 2 rows)
                TAY
                LDA     (byte_57),Y     ; Get byte from bottle 2 rows up from where byte $49 is.
                AND     #3              ; Mask off unnecessary bits.
                TAX
                LDA     byte_A22D,X     ; Get number from table indexed by contents of byte two rows up.
                ORA     byte_4C         ; OR in contents of $4C with number from table.
                STA     byte_4C         ; Store ORed data back to $4C.
                LDA     #0
                STA     byte_57         ; Clear byte $57.
                LDA     byte_49         ; Get screen location.
                CLC
                ADC     byte_4A         ; Add 16 to screen location. (move down 2 rows)
                TAY
                LDA     (byte_57),Y     ; Get byte from bottle 2 rows down from where byte $49 is.
                AND     #3              ; Mask off unnecessary bits.
                TAX
                LDA     byte_A22D,X     ; Get number from table indexed by contents of byte two rows down.
                ORA     byte_4C         ; OR in contents of $4C with number from table.
                STA     byte_4C         ; Store ORed data back to $4C.
                LDA     #0
                STA     byte_57         ; Clear byte $57.
                LDA     byte_49         ; Get screen location.
                AND     #7
                CMP     #2
                BCC     loc_9DC1        ; Branch if A < 2. (Screen location is in the left two columns)
                LDA     byte_49         ; Get screen location.
                SEC
                SBC     byte_4B         ; Subtract 2 from screen location. (move left 2 columns)
                TAY
                LDA     (byte_57),Y     ; Get byte from bottle 2 columns left from where byte $49 is.
                AND     #3              ; Mask off unnecessary bits.
                TAX
                LDA     byte_A22D,X     ; Get number from table indexed by contents of byte two columns left.
                ORA     byte_4C         ; OR in contents of $4C with number from table.
                STA     byte_4C         ; Store ORed data back to $4C.

loc_9DC1:                               ; CODE XREF: sub_9CFF+AEj
                LDA     #0
                STA     byte_57         ; Clear byte $57.
                LDA     byte_49         ; Get screen location.
                AND     #7
                CMP     #6
                BCS     loc_9DDF        ; Branch if A >= 6. (Screen location is in the right two columns)
                LDA     byte_49         ; Get screen location.
                CLC
                ADC     byte_4B         ; Add 2 to screen location. (move right 2 columns)
                TAY
                LDA     (byte_57),Y     ; Get byte from bottle 2 columns right from where byte $49 is.
                AND     #3              ; Mask off unnecessary bits.
                TAX
                LDA     byte_A22D,X     ; Get number from table indexed by contents of byte two columns right.
                ORA     byte_4C         ; OR in contents of $4C with number from table.
                STA     byte_4C         ; Store ORed data back to $4C.

loc_9DDF:                               ; CODE XREF: sub_9CFF+CCj sub_9CFF+FAj ...
                LDA     byte_4C
                CMP     #7
                BNE     loc_9DE8        ; Branch if 4 viruses don't surround the location $49 points to,
                                        ; with the 4 viruses containing all colors.
                JMP     loc_9D54
; ---------------------------------------------------------------------------

loc_9DE8:                               ; CODE XREF: sub_9CFF+E4j
                LDA     #0
                STA     byte_51         ; Clear byte $51.
                LDX     byte_48         ; Get integer in range [0,2].
                LDA     byte_A22D,X
                AND     byte_4C
                BEQ     loc_9E02
                DEC     byte_48
                LDA     byte_48
                BPL     loc_9DDF
                LDA     #2
                STA     byte_48
                JMP     loc_9DDF
; ---------------------------------------------------------------------------

loc_9E02:                               ; CODE XREF: sub_9CFF+F4j
                LDA     byte_49         ; Get new virus location
                STA     byte_57         ; Write new virus location
                LDY     #0              ; Always index from 0
                LDA     byte_48         ; Get new virus type
                ORA     #$D0            ; OR in for the name table
                STA     (byte_57),Y     ; Store a virus in the name table
                AND     #7
                TAX
                LDA     byte_72,X
                CLC
                ADC     #1
                STA     byte_72,X
                LDA     byte_316
                CMP     byte_396
                BNE     loc_9E29
                LDA     byte_48
                ORA     #$D0
                LDY     byte_49
                STA     unk_500,Y

loc_9E29:                               ; CODE XREF: sub_9CFF+11Fj
                INC     byte_A4
                LDA     byte_A4
                AND     #$F
                CMP     #$A
                BNE     loc_9E3A
                LDA     byte_A4
                CLC
                ADC     #6
                STA     byte_A4

loc_9E3A:                               ; CODE XREF: sub_9CFF+132j
                LDA     byte_57
                LSR     A
                LSR     A
                LSR     A
                STA     byte_80
                LDA     byte_A4
                LDA     #$10
                ORA     byte_52
                STA     byte_52
                DEC     REM_VIRUSES     ; Decrement remaining viruses to generate into the bottle.

locret_9E4B:                            ; CODE XREF: sub_9CFF+4j sub_9CFF+5Fj
                RTS
; End of function sub_9CFF
