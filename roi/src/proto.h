#ifndef _proto_h
#define _proto_h
#define CMD_NEW 0x01	# CMD | X | Y
#define CMD_MV 0x02	# CMD | ID | DX | DY
#define CMD_GONE 0x03	# CMD | ID
#define CMD_SC_ROI_ADD 0x04	# CMD | ID | NUM | TGT_ID | X | Y | TGT_ID | X | Y | ...
#define CMD_SC_ROI_RM 0x05	# CMD | ID | NUM | TGT_ID | X | Y | TGT_ID | X | Y | ...
#define CMD_SC_NEW 0x06		# CMD | ID
#define CMD_SC_ROI_MV 0x07	# CMD | ID | NUM | TGT_ID | X | Y | TGT_ID | X | Y | ...

#endif
