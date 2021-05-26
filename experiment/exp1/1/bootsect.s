!
!	bootsect.s		(C) 2021 JT Chen

SETUPLEN = 4				! nr of setup-sectors
BOOTSEG  = 0x07c0			! original address of boot-sector
INITSEG  = 0x9000			! we move boot here - out of the way
SETUPSEG = 0x9020			! setup starts here
SYSSEG   = 0x1000			! system loaded at 0x10000 (65536).
ENDSEG   = SYSSEG + SYSSIZE		! where to stop loading

! ROOT_DEV:	0x000 - same type of floppy as boot.
!		0x301 - first partition on first drive etc
ROOT_DEV = 0x306

entry _start
_start:

! Print some inane message

	mov	ah,#0x03		! read cursor pos
	xor	bh,bh
	int	0x10
	
	mov	cx,#24
	mov	bx,#0x0043		! psge 0, attribute 7 (normal)
	mov	bp,#msg1
	mov ax,#0x07c0
	mov es,ax
	mov	ax,#0x1301		! write string, move cursor
	int	0x10
	mov	bx,#0x0007

inf_loop:
	jmp inf_loop

load_setup:
	mov	dx,#0x0000		! drive 0, head 0
	mov	cx,#0x0002		! sector 2, track 0
	mov	bx,#0x0200		! address = 512, in INITSEG
	mov	ax,#0x0200+SETUPLEN	! service 2, nr of sectors
	int	0x13			! read it
	jnc	ok_load_setup		! ok - continue
	mov	dx,#0x0000
	mov	ax,#0x0000		! reset the diskette
	int	0x13
	j	load_setup

ok_load_setup:
	jmpi	0,SETUPSEG


msg1:
	.byte 13,10
	.ascii "JTOS is loading :) ....."
	.byte 13,10,13,10

boot_flag:
	.word 0xAA55

