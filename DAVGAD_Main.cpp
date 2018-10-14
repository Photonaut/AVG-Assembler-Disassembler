//---------------------------------------------------------------------------
//    D   A   V   G   A   D
//
// +Digital and Analogue Vector Generator Assembler / Disassembler


#include <vcl.h>
#include <string.h>
#include <stdio.h>
#include <process.h>
#include <set>

#pragma hdrstop

#include "DAVGAD_Main.h"
#include "Select_Game_dlg.h"

/*
0x188F 'S'
0x1894 'T'
0x182F 'A'
0x188B 'R'
0x187D ' '
0x18A0 'W'
*/


#define BITSET |=
#define BITCLR &= 0xFFFF -


#define MEM_OFFSET 0x0000
//#define MEM_OFFSET 0x1800

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;


HANDLE hEvent;
BOOL ThreadLoop;
DWORD ThreadHandle;

BYTE Vector_HEX_Data[16384];

#define CENTRE_X 512
#define CENTRE_Y 384

short New_X, New_Y;


class tVecSymbolTable
{
public:
 WORD Address;
 char strLabel[16];

friend bool operator==(const tVecSymbolTable& a, const tVecSymbolTable& b) {return(a.Address == b.Address);};
friend bool operator<(const tVecSymbolTable& a, const tVecSymbolTable& b) {return(a.Address < b.Address);};
tVecSymbolTable* GetMessageData(void) {return(this);};
};

std::set <tVecSymbolTable> Vector_Address_Label;

class tVecSymbolTable2
{
 public:
  WORD Address;
  char strLabel[16];

 friend bool operator==(const tVecSymbolTable2& a, const tVecSymbolTable2& b) {
 int Res;
  if(!strcmp(a.strLabel,b.strLabel)){
	 Res = 1;
  }else{
	 Res = 0;
  }
  return(Res);
 };

 friend bool operator<(const tVecSymbolTable2& a, const tVecSymbolTable2& b) {
  int Res;

  if (strcmp(a.strLabel,b.strLabel) < 0){
	 Res = 1;
  }else{
	 Res = 0;
  }

  return(Res);
 };
};

std::set <tVecSymbolTable2> Vector_Address_Label2;

//---------------------------------------------------------------------------


void ThreadFunc(PVOID pvoid)
{

 hEvent = CreateEvent(NULL,false,false,"");						// Create new event

 do{
	 WaitForSingleObject(hEvent, 100);				// Stall thread and wait for event release from either data byte or 100mS timeout
 }while(ThreadLoop);
 CloseHandle(hEvent);
}

//---------------------------------------------------------------------------


__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Draw_Hex_Grid(void)
{
 WORD Grid_Row, Grid_Col;
 WORD Checksum = 0;
 BYTE Vector_Hex_Data;
 char strCell_Text[32];

 for (Grid_Row=0; Grid_Row < (sgHex_Data->RowCount - 1); Grid_Row++) {

	wsprintf(strCell_Text, "%02X", Grid_Row * (sgHex_Data->ColCount - 1) );
	sgHex_Data->Cells[0][Grid_Row + 1] = strCell_Text;

	for (Grid_Col=0; Grid_Col < (sgHex_Data->ColCount -1); Grid_Col++) {
		Vector_Hex_Data = Vector_HEX_Data[(Grid_Row * (sgHex_Data->ColCount - 1)) + Grid_Col];
		Checksum += Vector_Hex_Data;
		wsprintf(strCell_Text, "%02X", Vector_Hex_Data);
		sgHex_Data->Cells[Grid_Col+1][Grid_Row+1] = strCell_Text;
	}
 }
 wsprintf(strCell_Text, "%04X", Checksum);
 lbChecksum->Caption = strCell_Text;
}

//---------------------------------------------------------------------------

void __fastcall TForm1::Find_Jump_Addresses(void)
{
 WORD Vector_PC;
 WORD Vector_Instruction;
 WORD Vector_Extra_Data;
 short X, Y;
 tVecSymbolTable Temp_Symbol;


 Vector_PC = 0x00;
 Vector_Address_Label.clear();               // Clear symbol table

 if (dlgGame_Select-> cbAVG_Selected -> Checked == TRUE){                      // AVG
	while(Vector_PC < 8192){
		if (dlgGame_Select->cbBig_Endian_6809->Checked==TRUE){
				Vector_Instruction = Vector_HEX_Data[(Vector_PC << 1) + 1] + ((WORD)Vector_HEX_Data[(Vector_PC << 1)] << 8);
        	}else{
                Vector_Instruction = Vector_HEX_Data[(Vector_PC << 1)] + ((WORD)Vector_HEX_Data[(Vector_PC << 1) + 1] << 8);
        	}

		  switch(Vector_Instruction & 0xE000){
                case 0x0000:                      // VCTR
                     Vector_PC++;
                       break;
                case 0xA000:                      // JSRL
						 Temp_Symbol.Address = Vector_Instruction & 0x1FFF;        // Store jump address
						 wsprintf(Temp_Symbol.strLabel, "L_%04X", Vector_Instruction & 0x1FFF);
						 Vector_Address_Label.insert(Temp_Symbol);
						 break;
                case 0xE000:                      // JMPL
                         Temp_Symbol.Address = Vector_Instruction & 0x1FFF;        // Store jump address
						 wsprintf(Temp_Symbol.strLabel, "L_%04X", Vector_Instruction & 0x1FFF);
                         Vector_Address_Label.insert(Temp_Symbol);                 // Insert symbol

                        break;
				default:
                        break;
          }
        Vector_PC++;
	 }

  }else{

 	while(Vector_PC < 4096){
		if (dlgGame_Select->cbBig_Endian_6809->Checked==TRUE){
                Vector_Instruction = Vector_HEX_Data[(Vector_PC << 1) + 1] + ((WORD)Vector_HEX_Data[(Vector_PC << 1)] << 8);
        }else{
                Vector_Instruction = Vector_HEX_Data[(Vector_PC << 1)] + ((WORD)Vector_HEX_Data[(Vector_PC << 1) + 1] << 8);
		}
                switch(Vector_Instruction & 0xF000){                    // DVG
                        case 0x0000:                      // VCTR0
                        case 0x1000:                      // VCTR1
						case 0x2000:                      // VCTR2
                        case 0x3000:                      // VCTR3
						case 0x4000:                      // VCTR4
						case 0x5000:                      // VCTR5
						case 0x6000:                      // VCTR6
						case 0x7000:                      // VCTR7
						case 0x8000:                      // VCTR8
						case 0x9000:                      // VCTR9
						case 0xA000:                      // LABS
						Vector_PC++;
						break;
				case 0xC000:                      // JRSL
						 Temp_Symbol.Address = Vector_Instruction & 0x0FFF;        // Store jump address
						 wsprintf(Temp_Symbol.strLabel, "L_%04X", Vector_Instruction & 0x0FFF);
						 Vector_Address_Label.insert(Temp_Symbol);
						 break;
				case 0xE000:                      // JMPL
						 Temp_Symbol.Address = Vector_Instruction & 0x0FFF;        // Store jump address
						 wsprintf(Temp_Symbol.strLabel, "L_%04X", Vector_Instruction & 0x0FFF);
						 Vector_Address_Label.insert(Temp_Symbol);                 // Insert symbol
						break;
				default:
						break;
				}
        		Vector_PC++;
 		}
	 }


 /*
 for ( location=Vector_Address_Label.begin() ; location != Vector_Address_Label.end(); location++ ){
        Memo1 -> Lines -> Add(location->strLabel);
 }
 */
}

void __fastcall TForm1::Disassemble_AVG(void)
{
 char str_Vector_Instruction[128];
 WORD Vector_PC;
 WORD Vector_Instruction;
 WORD Vector_Extra_Data;
 short X, Y;
 std::set<tVecSymbolTable>::iterator Label_Location;
 tVecSymbolTable Temp_Vec_Address;
 char strLabel[16];

 Vector_PC = 0x00;
 Memo1 -> Clear();                 // Clear Memo form
 Load_Progress_Value = 0;
 Memo1->Lines->BeginUpdate();
 while(Vector_PC < 8192){
	Load_Progress_Value = ((DWORD)Vector_PC * 100) / 8192;
	pbLoad_Progress->Position = Load_Progress_Value;

		if (dlgGame_Select->cbBig_Endian_6809->Checked==TRUE){
				Vector_Instruction = Vector_HEX_Data[(Vector_PC << 1) + 1] + ((WORD)Vector_HEX_Data[(Vector_PC << 1)] << 8);
		}else{
				Vector_Instruction = Vector_HEX_Data[(Vector_PC << 1)] + ((WORD)Vector_HEX_Data[(Vector_PC << 1) + 1] << 8);
		}

		Temp_Vec_Address.Address = Vector_PC;
		Label_Location = Vector_Address_Label.find(Temp_Vec_Address);

		if (Label_Location != Vector_Address_Label.end()){
		  strcpy(strLabel, Label_Location->strLabel);
		  strcat(strLabel, ":\t");
		}else{
		  strcpy(strLabel, "\t");
		}

		switch(Vector_Instruction & 0xE000){
		case 0x0000:                      // VCTR
//               Vector_Extra_Data = Vector_HEX_Data[((Vector_PC + 1) + 1) << 1] + ((WORD)Vector_HEX_Data[((Vector_PC+1) << 1)] << 8);
				Vector_PC++;
				if (dlgGame_Select->cbBig_Endian_6809->Checked==TRUE){
						Vector_Extra_Data = Vector_HEX_Data[(Vector_PC << 1) + 1] + ((WORD)Vector_HEX_Data[(Vector_PC << 1)] << 8);
				}else{
						Vector_Extra_Data = Vector_HEX_Data[(Vector_PC << 1)] + ((WORD)Vector_HEX_Data[(Vector_PC << 1) + 1] << 8);
				}
				   X = (Vector_Extra_Data & 0x1FFF);
				   Y = (Vector_Instruction & 0x1FFF);
				   if (X & 0x1000){
						X |= 0xE000;
				   }
				   if (Y & 0x1000){
						Y |= 0xE000;
				   }
					wsprintf(str_Vector_Instruction, "%sVCTR\t%d,%d,%d", strLabel,
					 X,
					 Y,
					 (Vector_Extra_Data & 0xE000) >> 13);

				break;

		case 0x2000:                      // HALT
			if (Vector_Instruction == 0x2020){
				wsprintf(str_Vector_Instruction, "%sHALT", strLabel);
			}else{
				wsprintf(str_Vector_Instruction, "%sDATA\t%04X", strLabel, Vector_Instruction);
			}
			break;

		case 0x4000:                      // SVEC
			   X = Vector_Instruction & 0x001F;
			   Y = (Vector_Instruction & 0x1F00) >> 8;
			   if (X & 0x10){
					X |= 0xFFE0;
			   }
			   if (Y & 0x10){
					Y |= 0xFFE0;
			   }
				wsprintf(str_Vector_Instruction, "%sSVEC\t%d,%d,%d", strLabel,
				 X,
				 Y,
				 (Vector_Instruction & 0x00E0) >> 5);
				break;

		case 0x6000:                      // STAT/SCAL
				if((Vector_Instruction & 0xF000) == 0x6000){    // If STAT then
					if(dlgGame_Select->rgVectorType -> ItemIndex == 0){            // If Star Wars type
							wsprintf(str_Vector_Instruction, "%sSTAT\t%02d,%02d", strLabel,
							(Vector_Instruction & 0x0F00) >> 8,
							Vector_Instruction & 0x00FF);
					}
					if(dlgGame_Select->rgVectorType -> ItemIndex == 1){            // If Battlezone type
							wsprintf(str_Vector_Instruction, "%sSTAT\t%02d,%02d,%02d", strLabel,
							Vector_Instruction & 0x00F,
							(Vector_Instruction & 0x00F0) >> 4,
							(Vector_Instruction & 0x0F00) >> 8);
					}
					if(dlgGame_Select->rgVectorType -> ItemIndex == 2){            // If Space Duel type
							wsprintf(str_Vector_Instruction, "%sSTAT\t%02d,%02d,%02d", strLabel,
							Vector_Instruction & 0x00F,
							(Vector_Instruction & 0x00F0) >> 4,
							(Vector_Instruction & 0x0F00) >> 8);
					}
				}else{                                          // Else SCAL
					if(Vector_Instruction & 0x0800){
						wsprintf(str_Vector_Instruction, "%sDATA\t%04X", strLabel, Vector_Instruction);
					}else{
						wsprintf(str_Vector_Instruction, "%sSCAL\t%02d,%02d",
						strLabel,
						(Vector_Instruction & 0x0700) >> 8,
						Vector_Instruction & 0x00FF);
					}

				}
				break;

		case 0x8000:                      // CNTR
			if ((Vector_Instruction & 0xFF00) == 0x8000){
				wsprintf(str_Vector_Instruction, "%sCNTR\t%02d", strLabel, Vector_Instruction & 0xFF);
			}else{
				wsprintf(str_Vector_Instruction, "%sDATA\t%04X", strLabel, Vector_Instruction);
			}
			break;

		case 0xA000:                      // JRSL
			 wsprintf(str_Vector_Instruction, "%sJRSL\tL_%04X", strLabel, Vector_Instruction & 0x1FFF);
			 break;

		case 0xC000:                      // RTSL
			if ((Vector_Instruction & 0xFFFF) == 0xC000){
				wsprintf(str_Vector_Instruction, "%sRTSL", strLabel);
			}else{
				wsprintf(str_Vector_Instruction, "%sDATA\t%04X", strLabel, Vector_Instruction);
			}
			break;

		case 0xE000:                      // JMPL
				wsprintf(str_Vector_Instruction, "%sJMPL\tL_%04X", strLabel, Vector_Instruction & 0x1FFF);
				break;

		default:
				wsprintf(str_Vector_Instruction, "%s--------------", strLabel);
				break;
		}
		Vector_PC++;

		Memo1 -> Lines -> Add(str_Vector_Instruction);
 }
 Memo1->Lines->EndUpdate();

 }

 // *************************************************

 void __fastcall TForm1::Disassemble_DVG(void)
{
 char str_Vector_Instruction[128];
 WORD Vector_PC;
 WORD Vector_Instruction;
 WORD Vector_Extra_Data;
 short Vect_Type;
 short X, Y;
 std::set<tVecSymbolTable>::iterator Label_Location;
 tVecSymbolTable Temp_Vec_Address;
 char strLabel[16];
 int SVec_Scale_Factor;

 char strAddress[16];

 Vector_PC = 0x00;
 Memo1 -> Clear();                 // Clear Memo form
 Load_Progress_Value = 0;
 Memo1->Lines->BeginUpdate();

 while(Vector_PC < 4096){
	Load_Progress_Value = ((DWORD)Vector_PC * 100) / 4096;
	pbLoad_Progress->Position = Load_Progress_Value;
		wsprintf(strAddress, "%04X", Vector_PC);

		if (dlgGame_Select->cbBig_Endian_6809->Checked==TRUE){
				Vector_Instruction = Vector_HEX_Data[(Vector_PC << 1) + 1] + ((WORD)Vector_HEX_Data[(Vector_PC << 1)] << 8);
		}else{
				Vector_Instruction = Vector_HEX_Data[(Vector_PC << 1)] + ((WORD)Vector_HEX_Data[(Vector_PC << 1) + 1] << 8);
		}

		Temp_Vec_Address.Address = Vector_PC;
		Label_Location = Vector_Address_Label.find(Temp_Vec_Address);

		if (Label_Location != Vector_Address_Label.end()){
		  strcpy(strLabel, Label_Location->strLabel);
		  strcat(strLabel, ":\t");
		}else{
		  strcpy(strLabel, "\t");
		}

		switch(Vector_Instruction & 0xF000){
		case 0x0000:                      // VCTR
		case 0x1000:                      // VCTR
		case 0x2000:                      // VCTR
		case 0x3000:                      // VCTR
		case 0x4000:                      // VCTR
		case 0x5000:                      // VCTR
		case 0x6000:                      // VCTR
		case 0x7000:                      // VCTR
		case 0x8000:                      // VCTR
		case 0x9000:                      // VCTR
//               Vector_Extra_Data = Vector_HEX_Data[((Vector_PC + 1) + 1) << 1] + ((WORD)Vector_HEX_Data[((Vector_PC+1) << 1)] << 8);

			Vector_PC++;
			   if (dlgGame_Select->cbBig_Endian_6809->Checked==TRUE){
						Vector_Extra_Data = Vector_HEX_Data[(Vector_PC << 1) + 1] + ((WORD)Vector_HEX_Data[(Vector_PC << 1)] << 8);
			   }else{
						Vector_Extra_Data = Vector_HEX_Data[(Vector_PC << 1)] + ((WORD)Vector_HEX_Data[(Vector_PC << 1) + 1] << 8);
			   }

			   if ((Vector_Instruction | Vector_Extra_Data) && !((Vector_Instruction & 0x0800) | (Vector_Extra_Data & 0x0800))){          // If not 0x0000 + 0x0000 empty data
				   X = (Vector_Extra_Data & 0x7FF);
				   Y = (Vector_Instruction & 0x7FF);
				   if (X & 0x0400){
						X = -(X & 0x3FF);         // Make value negative to print
				   }
				   if (Y & 0x0400){
						Y = -(Y & 0x3FF);         // Make value negative to print
				   }

				   Vect_Type = (Vector_Instruction & 0xF000) >> 12;

					wsprintf(str_Vector_Instruction, "%sVCTR%u\t%d, %d, %d", strLabel,
					 Vect_Type,
					 X,
					 Y,
					 (Vector_Extra_Data & 0xF000) >> 12);
			   }else{
					wsprintf(str_Vector_Instruction, "%sDATA\t%04X, %04X", strLabel, Vector_Instruction, Vector_Extra_Data);
			   }
				break;

		case 0xA000:                      // LABS
			Vector_PC++;
			   if (dlgGame_Select->cbBig_Endian_6809->Checked==TRUE){
						Vector_Extra_Data = Vector_HEX_Data[(Vector_PC << 1) + 1] + ((WORD)Vector_HEX_Data[(Vector_PC << 1)] << 8);
			   }else{
						Vector_Extra_Data = Vector_HEX_Data[(Vector_PC << 1)] + ((WORD)Vector_HEX_Data[(Vector_PC << 1) + 1] << 8);
			   }
			   if ((Vector_Instruction | Vector_Extra_Data) && !((Vector_Instruction & 0x0800) | (Vector_Extra_Data & 0x0800))){          // If not 0x0000 + 0x0000
					   X = (Vector_Extra_Data & 0x7FF);
					   Y = (Vector_Instruction & 0x7FF);
					   if (X & 0x0400){
							X = -X;         // Make value negative to print
					   }
					   if (Y & 0x0400){
							Y = -Y;         // Make value negative to print
					   }
						wsprintf(str_Vector_Instruction, "%sLABS\t%d, %d, %d", strLabel,
						 X,
						 Y,
						 (Vector_Extra_Data & 0xF000) >> 12);

				}else{
					wsprintf(str_Vector_Instruction, "%sDATA\t%04X, %04X", strLabel, Vector_Instruction, Vector_Extra_Data);
				}
				break;

		case 0xB000:                      // HALT
				if (Vector_Instruction == 0xB0B0){
					wsprintf(str_Vector_Instruction, "%sHALT", strLabel);
				}else{
					wsprintf(str_Vector_Instruction, "%sDATA\t%04X", strLabel, Vector_Instruction);
				}
				break;
		case 0xC000:                      // JRSL
				 wsprintf(str_Vector_Instruction, "%sJRSL\tL_%04X", strLabel, Vector_Instruction & 0x0FFF);
				 break;
		case 0xD000:                      // RTSL
				if (Vector_Instruction == 0xD000){
					wsprintf(str_Vector_Instruction, "%sRTSL", strLabel);
				}else{
					wsprintf(str_Vector_Instruction, "%sDATA\t%04X", strLabel, Vector_Instruction);
				}
				break;
		case 0xE000:                      // JMPL
				wsprintf(str_Vector_Instruction, "%sJMPL\tL_%04X", strLabel, Vector_Instruction & 0x0FFF);
				break;
		case 0xF000:                      // SVEC X, Y, Z Int, Scal
			   Y = (Vector_Instruction & 0x700) >> 8;
			   X = Vector_Instruction & 0x007;
			   if (X & 0x04){       // If negative then
					X = -(X & 0x03);         // Make value negative to print
			   }
			   if (Y & 0x04){
					Y = -(Y & 0x03);
			   }
			   SVec_Scale_Factor = ((Vector_Instruction & 0x08) >> 2) | ((Vector_Instruction & 0x0800) >> 11);
			   wsprintf(str_Vector_Instruction, "%sSVEC\t%d, %d, %d, %d", strLabel,
				 X,
                 Y,
                 (Vector_Instruction & 0x00F0) >> 4,
                 SVec_Scale_Factor);


                break;
        default:
                wsprintf(str_Vector_Instruction, "%s--------------", strLabel);
				break;
		}

        Vector_PC++;

		Memo1 -> Lines -> Add(str_Vector_Instruction);
 }
 Memo1->Lines->EndUpdate();
 }
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
 HANDLE hRead_File;
 DWORD Bytes_Read;

 if(OpenDialog1 -> Execute()){
	 Label1 -> SetTextBuf(OpenDialog1 -> FileName.c_str());		// Show file name
	 if ((hRead_File = CreateFile(AnsiString(OpenDialog1 -> FileName).c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE) {

	 Button1 -> Enabled = FALSE;
	 Button2 -> Enabled = FALSE;
	 Button3 -> Enabled = FALSE;
	 Button4 -> Enabled = FALSE;
	 Button5 -> Enabled = FALSE;
	 bnDraw_Button -> Enabled = FALSE;

	if (dlgGame_Select->cbAVG_Selected->Checked) {
		ReadFile(hRead_File, &Vector_HEX_Data, 0x4000, &Bytes_Read, NULL);
	}else{
		ReadFile(hRead_File, &Vector_HEX_Data, 0x2000, &Bytes_Read, NULL);
	}

	 Draw_Hex_Grid();

	 CloseHandle(hRead_File);
	 Find_Jump_Addresses();
	 if (dlgGame_Select->cbAVG_Selected -> Checked == TRUE){
			Disassemble_AVG();
	 }else{
			Disassemble_DVG();
	 }
	 Button1 -> Enabled = TRUE;
	 Button2 -> Enabled = TRUE;
	 Button3 -> Enabled = TRUE;
	 Button4 -> Enabled = TRUE;
	 Button5 -> Enabled = TRUE;
	 bnDraw_Button -> Enabled = TRUE;
     bnDraw_ButtonClick(Form1);
	 }
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button2Click(TObject *Sender)
{
 HANDLE hWrite_File;
 DWORD Bytes_Read;

 if(SaveDialog1 -> Execute()){
		Label1 -> SetTextBuf(SaveDialog1 -> FileName.c_str());		// Show file name

		 if ((hWrite_File = CreateFile(AnsiString(SaveDialog1 -> FileName).c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE) {
			if (dlgGame_Select->cbAVG_Selected->Checked) {
				WriteFile(hWrite_File, &Vector_HEX_Data, 0x4000, &Bytes_Read, NULL);
			}else{
				WriteFile(hWrite_File, &Vector_HEX_Data, 0x2000, &Bytes_Read, NULL);
			}

			  CloseHandle(hWrite_File);
        }

 }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
 ThreadLoop = FALSE;
 SetEvent(hEvent);                      // Stop thread

}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormShow(TObject *Sender)
{
 dlgGame_Select->ShowModal();

 if(dlgGame_Select->rgVectorType->ItemIndex == 3){
	sgHex_Data->RowCount = 257;
	btConvert_PLT ->Enabled = FALSE;
 }else{
	sgHex_Data->RowCount = 513;
	btConvert_PLT ->Enabled = TRUE;
 }

 Timer1 -> Enabled = TRUE;
 ThreadLoop = TRUE;
//     ThreadHandle = _beginthread(ThreadFunc,0,NULL);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
 Programmer_Mode = FALSE;
 if (ParamCount()){
     if(ParamStr(1) == "/p"){                // If command line switch is /p then
          Programmer_Mode = TRUE;            // Flag to show zero buttons
     }
 }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Draw_AVG(void)
{
 TCanvas *pCanvas = Image1->Canvas;
 BOOL Keep_Looping = TRUE;
 WORD Vector_Instruction = 0;
 WORD Vector_Extra_Data;
 WORD Vector_PC;
 short X, Y;
 BYTE Colour;
 BYTE Intensity = 1;
 BYTE VIntensity;
 BYTE Stat;
 WORD Scale = 128;
 BYTE Bin_Scale;
 WORD Bin_Scaler = 256;
 static WORD Stack[4];
 static BYTE Stack_Index = 0;
 WORD Run_Timeout = 50000;

  pCanvas->Pen->Color = clBlack;
  pCanvas->Brush->Color = clBlack;
//  pCanvas->Brush->Style = bsDiagCross;
  pCanvas->Rectangle(0, 0, Image1->Width, Image1->Height);

  if (CheckBox1 -> Checked == TRUE){
	 Colour = 7;
	 VIntensity = 0x07;
	 Intensity = 0xFF;
	 Bin_Scale = 1;
  }

 sscanf(AnsiString(Edit1 -> Text).c_str(), "%X",  &Vector_PC);
 Vector_PC -= MEM_OFFSET;

 pCanvas->MoveTo(512, 384);
 New_X = 512;
 New_Y = 384;

 while((Vector_PC < 8192) && Keep_Looping && Run_Timeout--){

		if (dlgGame_Select->cbBig_Endian_6809->Checked==TRUE){
				Vector_Instruction = Vector_HEX_Data[(Vector_PC << 1) + 1] + ((WORD)Vector_HEX_Data[(Vector_PC << 1)] << 8);
		}else{
				Vector_Instruction = Vector_HEX_Data[(Vector_PC << 1)] + ((WORD)Vector_HEX_Data[(Vector_PC << 1) + 1] << 8);
		}

		switch(Vector_Instruction & 0xE000){
		case 0x0000:                      // VCTR
				Vector_PC++;
				if (dlgGame_Select->cbBig_Endian_6809->Checked==TRUE){
						Vector_Extra_Data = Vector_HEX_Data[(Vector_PC << 1) + 1] + ((WORD)Vector_HEX_Data[(Vector_PC << 1)] << 8);
				}else{
						Vector_Extra_Data = Vector_HEX_Data[(Vector_PC << 1)] + ((WORD)Vector_HEX_Data[(Vector_PC << 1) + 1] << 8);
				}

				switch (dlgGame_Select->rgVectorType -> ItemIndex){
				case 0:
					VIntensity = ((DWORD)(Vector_Extra_Data & 0xE000) * 0x123 * Intensity) >> 16;     // Star Wars
					break;
				case 1:
					VIntensity = ((DWORD)(Vector_Extra_Data & 0xE000) * 0x123 * Intensity) >> 16;     // Battlezone
					break;
				case 2:
					if((Vector_Extra_Data & 0xE000) == 0x1000){
						VIntensity = Intensity;
					}else{
						VIntensity = ((DWORD)(Vector_Extra_Data & 0xE000) * 0x123) >> 16;     // SD/G/BW
					}
					break;

				default:
					break;
				}


                if (VIntensity){
                        switch(Colour){
						case 0:
                                pCanvas->Pen->Color = clBlack;
                                break;
                        case 1:
                                pCanvas->Pen->Color = (DWORD)VIntensity << 16;
								break;
                        case 2:
                                pCanvas->Pen->Color = (DWORD)VIntensity << 8;
                                break;
						case 3:
                                pCanvas->Pen->Color = ((DWORD)VIntensity << 16) | ((DWORD)VIntensity << 8);
                                break;
                        case 4:
								pCanvas->Pen->Color = VIntensity;
								break;
                        case 5:
                                pCanvas->Pen->Color = ((DWORD)VIntensity << 16) | VIntensity;
								break;
                        case 6:
                                pCanvas->Pen->Color = VIntensity | ((DWORD)VIntensity << 8);
                                break;
						case 7:
								pCanvas->Pen->Color = ((DWORD)VIntensity << 16) | ((DWORD)VIntensity << 8) | VIntensity;
                                break;
                        }
				}else{
				  if (CheckBox1 -> Checked == TRUE){
                        pCanvas->Pen->Color = 0x404040;
				  }else{
                        pCanvas->Pen->Color = clBlack;
				  }
                }

				X = Vector_Extra_Data & 0x1FFF;
				if (X & 0x1000){
						X = (X | 0xE000);
				}
				Y = Vector_Instruction & 0x1FFF;
				if (Y & 0x1000){
						Y = (Y | 0xE000);
				}

				New_X = New_X + (((long)X * Bin_Scaler * Scale) / (128 * 256));
				New_Y = New_Y - (((long)Y * Bin_Scaler * Scale) / (192 * 256));

				if (pCanvas->Pen->Color == clBlack){
						pCanvas->MoveTo(New_X, New_Y);
				}else{
						pCanvas->LineTo(New_X, New_Y);
						pCanvas->Pixels[New_X][New_Y] = pCanvas->Pen->Color;
				}

				Vector_PC++;
				break;
		case 0x2000:                      // HALT
				Keep_Looping = FALSE;
				break;

		case 0x4000:                      // SVEC
/*				if(dlgGame_Select->rgVectorType -> ItemIndex == 0){            // If Star Wars type
					VIntensity = ((((Vector_Extra_Data & 0xE000)) >> 13) * Intensity) >> 2;     // Works OK for Star Wars
				}
				if(dlgGame_Select->rgVectorType -> ItemIndex == 2){            // If Space Duel/Gravitar/Black Widow type
					if((Vector_Extra_Data & 0xE000) == 0x1000){
						VIntensity = Intensity;
					}else{
						VIntensity = ((Vector_Extra_Data & 0xE000)) >> 8;     // Gravitar/SD/BW.
					}
				}*/
			switch(dlgGame_Select->rgVectorType -> ItemIndex){
			case 0:
				VIntensity = ((DWORD)(Vector_Instruction & 0x00E0) * 0x123 * Intensity) >> 16;     // Works OK for Star Wars
				break;
			case 1:
				VIntensity = ((Vector_Instruction & 0x00E0) * 0x123) >> 8;     // Gravitar/SD/BW.
				break;
			case 2:
				VIntensity = ((Vector_Instruction & 0x00E0) * 0x123) >> 8;     // Gravitar/SD/BW.
				break;
			}

				if (VIntensity){
						switch(Colour){
						case 0:
								pCanvas->Pen->Color = clBlack;
								break;
						case 1:
								pCanvas->Pen->Color = (DWORD)VIntensity << 16;
								break;
						case 2:
								pCanvas->Pen->Color = (DWORD)VIntensity << 8;
								break;
						case 3:
								pCanvas->Pen->Color = ((DWORD)VIntensity << 16) | ((DWORD)VIntensity << 8);
								break;
						case 4:
								pCanvas->Pen->Color = VIntensity;
								break;
						case 5:
								pCanvas->Pen->Color = ((DWORD)VIntensity << 16) | VIntensity;
								break;
						case 6:
								pCanvas->Pen->Color = VIntensity | ((DWORD)VIntensity << 8);
								break;
						case 7:
								pCanvas->Pen->Color = ((DWORD)VIntensity << 16) | ((DWORD)VIntensity << 8) | VIntensity;
								break;
						}
				}else{
				  if (CheckBox1 -> Checked == TRUE){
						pCanvas->Pen->Color = 0x404040;
				  }else{
						pCanvas->Pen->Color = clBlack;
				  }
				}

				X = Vector_Instruction & 0x001F;
				if (X & 0x10){
					X = (X | 0xFFE0);
 //					X = -X;
				}
				Y = (Vector_Instruction & 0x1F00) >> 8;
				if (Y & 0x10){
					Y = (Y | 0xFFE0);
//					Y = -Y;
				}

				New_X = New_X + (((long)X * Bin_Scaler * Scale) / (64 * 256));
				New_Y = New_Y - (((long)Y * Bin_Scaler * Scale) / (96 * 256));

				if (pCanvas->Pen->Color == clBlack){
						pCanvas->MoveTo(New_X, New_Y);
				}else{
						pCanvas->LineTo(New_X, New_Y);
						pCanvas->Pixels[New_X][New_Y] = pCanvas->Pen->Color;
				}
				Vector_PC++;
				break;


		case 0x6000:                      // STAT/SCAL
				if((Vector_Instruction & 0xF000) == 0x6000){    // If STAT then
					switch(dlgGame_Select->rgVectorType -> ItemIndex ){

//						if(dlgGame_Select->rgVectorType -> ItemIndex == 0){            // If Star Wars type
					case 0:
						Intensity = (Vector_Instruction & 0x00FF);
						Colour = (Vector_Instruction & 0x0700) >> 8;
//						}
						break;
//						if(dlgGame_Select->rgVectorType -> ItemIndex == 1){            // If  Battlezone type
					case 1:
						Colour = 0x07;
						Intensity = 0xFF;               // Battlezone fudge
//						}
						break;
//						if(dlgGame_Select->rgVectorType -> ItemIndex == 2){            // If Space Duel type
					case 2:
						Colour = (Vector_Instruction & 0x0007);
						Intensity = (Vector_Instruction & 0x00F0) | ((Vector_Instruction & 0x00F0) >> 4);
//                        }
						break;
					}

				}else{                                          // Else SCAL
					Scale = ((Vector_Instruction & 0x00FF) ^ 0xFF) + 1;
					Bin_Scale = (Vector_Instruction & 0x0700) >> 8;

					Bin_Scaler = 512;
					Bin_Scaler >>= Bin_Scale;
					if (dlgGame_Select->cbBig_Endian_6809 -> Checked == FALSE){           // Fudge for Battlezone needs fixing
						Bin_Scaler >>=  1;
					}

				}
				Vector_PC++;
				break;

		case 0x8000:                      // CNTR
				pCanvas->MoveTo(CENTRE_X, CENTRE_Y);
				New_X = CENTRE_X;
                New_Y = CENTRE_Y;
                Vector_PC++;
                break;

        case 0xA000:                      // JRSL
				Stack[Stack_Index++] = Vector_PC + 1;
				if(Stack_Index > 3){
					Stack_Index = 0;
				}
				Vector_PC = Vector_Instruction & 0x1FFF - MEM_OFFSET;
				break;

		case 0xC000:                      // RTSL
				if(Stack_Index == 0 ){
					Stack_Index = 4;
				}
				Vector_PC = Stack[--Stack_Index] - MEM_OFFSET;
				break;

		case 0xE000:                      // JMPL
                Vector_PC = Vector_Instruction & 0x1FFF - MEM_OFFSET;
                break;

		default:
                Keep_Looping = FALSE;
                break;
		}


 }
}

//---------------------------------------------------------------------------

void __fastcall TForm1::Draw_DVG(void)
{
 TCanvas *pCanvas = Image1->Canvas;
 BOOL Keep_Looping = TRUE;
 WORD Vector_Instruction = 0;
 WORD Vector_Extra_Data;
 WORD Vector_PC;
 short X, Y;
 BYTE Colour;
 BYTE Intensity = 1;
 BYTE VIntensity;
 BYTE Stat;
 WORD Scale = 128;
 int SVec_Scale_Factor;
 int i, iScale;
 BYTE Bin_Scale;
 WORD Bin_Scaler = 256;
 WORD Global_Scale;
 static WORD Stack[4];
 static BYTE Stack_Index = 0;
 WORD Run_Timeout = 50000;


 WORD LABS_Global_Scale;
 WORD Vctr_Local_Scale;
 WORD New_Scale;

  pCanvas->Pen->Color = clBlack;
  pCanvas->Brush->Color = clBlack;
  pCanvas->Rectangle(0, 0, Image1->Width, Image1->Height);

  if (CheckBox1 -> Checked == TRUE){
	 Colour = 7;
	 VIntensity = 0x07;
	 Intensity = 0xFF;
	 Bin_Scale = 1;
  }

 sscanf(AnsiString(Edit1 -> Text).c_str(), "%X",  &Vector_PC);
 Vector_PC -= MEM_OFFSET;

 pCanvas->MoveTo(CENTRE_X, CENTRE_Y);           // Start in midle of screen
 New_X = CENTRE_X;
 New_Y = CENTRE_Y;

 while((Vector_PC < 4096) && Keep_Looping && Run_Timeout--){

		if (dlgGame_Select->cbBig_Endian_6809->Checked==TRUE){
				Vector_Instruction = Vector_HEX_Data[(Vector_PC << 1) + 1] + ((WORD)Vector_HEX_Data[(Vector_PC << 1)] << 8);
		}else{
				Vector_Instruction = Vector_HEX_Data[(Vector_PC << 1)] + ((WORD)Vector_HEX_Data[(Vector_PC << 1) + 1] << 8);
		}

		switch(Vector_Instruction & 0xF000){
		case 0x0000:                      // VCTR
		case 0x1000:                      // VCTR
		case 0x2000:                      // VCTR
		case 0x3000:                      // VCTR
		case 0x4000:                      // VCTR
		case 0x5000:                      // VCTR
		case 0x6000:                      // VCTR
		case 0x7000:                      // VCTR
		case 0x8000:                      // VCTR
		case 0x9000:                      // VCTR

			Vector_PC++;
			if (dlgGame_Select->cbBig_Endian_6809->Checked==TRUE){
				Vector_Extra_Data = Vector_HEX_Data[(Vector_PC << 1) + 1] + ((WORD)Vector_HEX_Data[(Vector_PC << 1)] << 8);
			}else{
				Vector_Extra_Data = Vector_HEX_Data[(Vector_PC << 1)] + ((WORD)Vector_HEX_Data[(Vector_PC << 1) + 1] << 8);
			}

			VIntensity = (Vector_Extra_Data & 0xF000) >> 8;

//			Bin_Scale = ((Vector_Instruction & 0xF000) >> 12);
//			Bin_Scaler = 512;
//			Bin_Scaler >>= (9 - Bin_Scale);


			Vctr_Local_Scale = Vector_Instruction >> 12;        // Get scale from vector instruction


				if (VIntensity){
					pCanvas->Pen->Color = ((DWORD)VIntensity << 16) | ((DWORD)VIntensity << 8) | VIntensity;
				}else{
					if (CheckBox1 -> Checked == TRUE){
						pCanvas->Pen->Color = 0x404040;
					}else{
						pCanvas->Pen->Color = clBlack;
					}
				}

				X = (Vector_Extra_Data & 0x7FF);
				Y = (Vector_Instruction & 0x7FF);
				if (X & 0x0400){
//                        X |= 0xF800;
					X = -(X & 0x3FF);
				}
				if (Y & 0x0400){
//                        Y |= 0xF800;
					Y = -(Y & 0x3FF);
				}

				New_Scale  = ((Vctr_Local_Scale + LABS_Global_Scale) & 0x0f);
				New_X  += X >> (9-New_Scale);
				New_Y  -= Y >> (9-New_Scale);


//				New_X = New_X + (((long)X * Bin_Scaler * Global_Scale) / (128 * 512));
//				New_Y = New_Y - (((long)Y * Bin_Scaler * Global_Scale) / (192 * 512));

				if (pCanvas->Pen->Color == clBlack){
						pCanvas->MoveTo(New_X, New_Y);
				}else{
						pCanvas->LineTo(New_X, New_Y);
						pCanvas->Pixels[New_X][New_Y] = pCanvas->Pen->Color;
				}

				Vector_PC++;
				break;

		case 0xA000:                      // LABS
				Vector_PC++;

				if (dlgGame_Select->cbBig_Endian_6809->Checked==TRUE){
						Vector_Extra_Data = Vector_HEX_Data[(Vector_PC << 1) + 1] + ((WORD)Vector_HEX_Data[(Vector_PC << 1)] << 8);
				}else{
						Vector_Extra_Data = Vector_HEX_Data[(Vector_PC << 1)] + ((WORD)Vector_HEX_Data[(Vector_PC << 1) + 1] << 8);
				}

/*				if(Vector_Extra_Data >> 12 & 0x08){
						Global_Scale = 256 >> (8 - ((Vector_Extra_Data >> 12) & 0x07));
				}else{
						Global_Scale = 256 << ((Vector_Extra_Data >> 12) & 0x07);
				}*/
				LABS_Global_Scale = Vector_Extra_Data >> 12;    // Get nibble for global scale


  //				 iScale = (Vector_Extra_Data & 0xf000) >> 12;
//				 if (Vector_Extra_Data & 0x8000) 	// divisor = negative shift
//					iScale = iScale - 16;			// two's complement negative #


				X = (Vector_Extra_Data & 0x7FF);
				Y = (Vector_Instruction & 0x07FF);
				if (X & 0x0400){
//                        X |= 0xF800;
					X = -(X & 0x3FF);
				}
				if (Y & 0x0400){
//						Y |= 0xF800;
					Y = -(Y & 0x3FF);
				}

				New_X = X ;
				New_Y = 896 - Y;

				pCanvas->MoveTo(New_X, New_Y);

				Vector_PC++;
				break;

		case 0xB000:                      // HALT
				Keep_Looping = FALSE;
				break;

		case 0xC000:                      // JRSL
				Stack[Stack_Index++] = Vector_PC + 1;
				Stack_Index &= 0x03;
				Vector_PC = Vector_Instruction & 0x0FFF - MEM_OFFSET;
				break;

		case 0xD000:                      // RTSL
				Stack_Index -= 1;
				Stack_Index &= 0x03;
				Vector_PC = Stack[Stack_Index] - MEM_OFFSET;
				break;

		case 0xE000:                      // JMPL
				Vector_PC = Vector_Instruction & 0x0FFF - MEM_OFFSET;
				break;

		case 0xF000:                      // SVEC
				VIntensity = (Vector_Instruction & 0x00F0);
				if (VIntensity){
					pCanvas->Pen->Color = ((DWORD)VIntensity << 16) | ((DWORD)VIntensity << 8) | VIntensity;
				}else{
					if (CheckBox1 -> Checked == TRUE){
						pCanvas->Pen->Color = 0x404040;
					}else{
						pCanvas->Pen->Color = clBlack;
					}
				}


//				 i = 2 + ((Vector_Instruction >> 2) & 0x02) + ((Vector_Instruction >> 11) & 0x01);  // Get scale from SVEC 2 bits
//				 i = ((iScale + i) & 0x0f);
//				 if (i > 9){
//					i = -1;
//				 }

				Vctr_Local_Scale = 2 + ((Vector_Instruction >> 2) & 0x02) + ((Vector_Instruction >> 11) & 0x01);        // Get scale from vector instruction

				 Y = Vector_Instruction & 0x300;
				 if (Vector_Instruction & 0x400){ 		// X sign bit
					Y = -Y;
				 }

				 X = (Vector_Instruction & 0x3) << 8;
				 if (Vector_Instruction & 0x004){ 	// Y sign bit
					X = -X;
				 }

//				 New_X  += (X << 0) >> (9-i);
//				 New_Y  -= (Y << 0) >> (9-i);

				New_Scale  = ((Vctr_Local_Scale + LABS_Global_Scale) & 0x0f);
				New_X  += X   >> (9-New_Scale);
				New_Y  -= Y >> (9-New_Scale);




				if (pCanvas->Pen->Color == clBlack){
						pCanvas->MoveTo(New_X, New_Y);
				}else{
						pCanvas->LineTo(New_X, New_Y);
						pCanvas->Pixels[New_X][New_Y] = pCanvas->Pen->Color;
				}

				Vector_PC++;
				break;

		default:
				Keep_Looping = FALSE;
				break;
		}

 }
}

//---------------------------------------------------------------------------

void __fastcall TForm1::Image1Click(TObject *Sender)
{
 TCanvas *pCanvas = Image1->Canvas;

 pCanvas->MoveTo(256, 192);
 New_X = 256;
 New_Y = 192;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Memo1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
 long Line;
 char s[128];
 WORD Vec_Addr;

// Line = Memo1->CaretPos.y;
 Line = Memo1->SelStart;

 Label2 -> Caption = X;
 Label3 -> Caption = Y;
 Label4 -> Caption = Line;

/* Label2->Caption = Line;
 strcpy(s, Memo1-> Lines -> Strings[Line].c_str());
 sscanf(s, "%X ", &Vec_Addr);
 wsprintf(s, "%04X", Vec_Addr);

 Edit1-> Text = s;*/
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{
 HANDLE hWrite_File;
 DWORD Bytes_Read;

 if(SaveDialog2 -> Execute()){
		Label1 -> SetTextBuf(SaveDialog2 -> FileName.c_str());		// Show file name

		 if ((hWrite_File = CreateFile(AnsiString(SaveDialog2 -> FileName).c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE) {
              WriteFile(hWrite_File, AnsiString(Memo1->Text).c_str(), Memo1->GetTextLen(), &Bytes_Read, NULL);
                CloseHandle(hWrite_File);
        }

 }

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{
 HANDLE hRead_File;
 DWORD Bytes_Read;
 char Buffer[65536 * 8];

 if(OpenDialog2 -> Execute()){
		Label1 -> SetTextBuf(OpenDialog2 -> FileName.c_str());		// Show file name
 }
 if ((hRead_File = CreateFile(AnsiString(OpenDialog2 -> FileName).c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE) {
		Memo1 -> Clear();                 // Clear Memo form
        memset(Buffer, 0, sizeof(Buffer));
		ReadFile(hRead_File, Buffer, 65535*8, &Bytes_Read, NULL);
		Memo1->Text = Buffer;

        CloseHandle(hRead_File);
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Write_Instruction_HEX_Data(WORD Vector_PC, WORD Vector_Instruction_Data)
{
 char strError[128];
 WORD Old_Instruction;

 if (dlgGame_Select->cbBig_Endian_6809 -> Checked == FALSE){
	Old_Instruction = Vector_HEX_Data[Vector_PC] | (WORD)Vector_HEX_Data[Vector_PC + 1] << 8;
	if((Vector_Instruction_Data != Old_Instruction) && cbError_Check->Checked){
		wsprintf(strError, "Address:%04X Old:%04X New:%04X", Vector_PC >> 1, Old_Instruction, Vector_Instruction_Data);
		lbAssemble_Error->Caption = strError;
		Assembly_Error = TRUE;
	}

	 Vector_HEX_Data[Vector_PC + 1] = Vector_Instruction_Data >> 8;
	 Vector_HEX_Data[Vector_PC] = Vector_Instruction_Data & 0xFF;
 }else{
	Old_Instruction = Vector_HEX_Data[Vector_PC + 1] | (WORD)Vector_HEX_Data[Vector_PC] << 8;
	if((Vector_Instruction_Data != Old_Instruction) && cbError_Check->Checked){
		wsprintf(strError, "Address:%04X Old:%04X New:%04X", Vector_PC >> 1, Old_Instruction, Vector_Instruction_Data);
		lbAssemble_Error->Caption = strError;
		Assembly_Error = TRUE;
	}
	 Vector_HEX_Data[Vector_PC] = Vector_Instruction_Data >> 8;
	 Vector_HEX_Data[Vector_PC + 1] = Vector_Instruction_Data & 0xFF;
 }
}

//---------------------------------------------------------------------------


void __fastcall TForm1::Image1MouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
 Label2 -> Caption = X - CENTRE_X;
 Label3 -> Caption = Y - CENTRE_Y;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::bnDraw_ButtonClick(TObject *Sender)
{
 if (dlgGame_Select->cbAVG_Selected -> Checked == TRUE){
		Draw_AVG();
 }else{
		Draw_DVG();
 }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Assemble_AVG(void)
{
 WORD Vector_PC;
 WORD Vector_Instruction;
 WORD Vector_Extra_Data;
 WORD Token_Count;
 int Count;
 tVecSymbolTable2 Temp_Label;
 std::set<tVecSymbolTable2>::iterator Label_Location;
 WORD Invalid_Label_Address;
 int Assembly_Source_Line;
 char strLabel[32];
 char strInstr[16];
 char strOp1[16];
 char strOp2[16];
 char strOp3[16];
 char * ptrToken;
 char strLine_Buffer[128];
 char Pass;


 Vector_Address_Label2.clear();               // Clear symbol table

 for (Pass = 0; Pass < 2; Pass++){
 Vector_PC = 0;
 for (Assembly_Source_Line = 0;Assembly_Source_Line < Memo1 -> Lines->Count;Assembly_Source_Line++){                      // For each line
	lbAssembly_Line->Caption = Memo1->Lines->Strings[Assembly_Source_Line];//Assembly_Source_Line;
	 strcpy(strLine_Buffer, AnsiString(Memo1 -> Lines->Strings[Assembly_Source_Line]).c_str());
	 strcpy(strInstr, "");             // Flag that no instrucion
	 strcpy(strOp1, "");
	 strcpy(strOp2, "");
	 strcpy(strOp3, "");

	 if ((strLine_Buffer[0] != '\t') && (strLine_Buffer[0] != ' ') && (strLine_Buffer[0] != '\n') && (strLine_Buffer[0] != '\r')){                        // If a label detected then
			   if((ptrToken = strtok(strLine_Buffer, ": \t\n\r;")) != NULL){
					strcpy(strLabel, ptrToken);             // Copy label string

					if((ptrToken = strtok(NULL, " \t\n\r;")) != NULL){
						 strcpy(strInstr, ptrToken);             // Copy instruction string
						 Token_Count = 0;
					}else{
						 strcpy(strInstr, "");             // Flag that no instrucion
					}
					if((ptrToken = strtok(NULL, ", \t\n\r;")) != NULL){
						 strcpy(strOp1, ptrToken);             // Copy Operand 1 string
						 Token_Count = 1;
					}
					if((ptrToken = strtok(NULL, ", \t\n\r;")) != NULL){
						strcpy(strOp2, ptrToken);             // Copy Operand 2 string
						Token_Count = 2;
					}
					if((ptrToken = strtok(NULL, " \t\n\r;")) != NULL){
						 strcpy(strOp3, ptrToken);             // Copy Operand 3 string
						 Token_Count = 3;
					}
					if (Pass == 0){
						 strcpy(Temp_Label.strLabel, strLabel);
						 Temp_Label.Address = Vector_PC;
						 Vector_Address_Label2.insert(Temp_Label);
					}
			   }
	 }else{
					if((ptrToken = strtok(strLine_Buffer, " \t\n\r;")) != NULL){
						 strcpy(strInstr, ptrToken);             // Copy instruction string
						 Token_Count = 0;
					}else{
						 strcpy(strInstr, "");             // Flag that no instrucion
					}
					if((ptrToken = strtok(NULL, ", \t\n\r;")) != NULL){
						 strcpy(strOp1, ptrToken);             // Copy Operand 1 string
						 Token_Count = 1;
					}
					if((ptrToken = strtok(NULL, ", \t\n\r;")) != NULL){
						 strcpy(strOp2, ptrToken);             // Copy Operand 2 string
						 Token_Count = 2;
					}
					if((ptrToken = strtok(NULL, " \t\n\r;")) != NULL){
						 strcpy(strOp3, ptrToken);             // Copy Operand 3 string
						 Token_Count = 3;
					}
	 }

	 if (strInstr[0] != 0){          // If instruction detected then

		  if (!strcmp(strInstr, "ORG")){          // Change vector address PC
			   Vector_PC = strtol(strOp1, NULL, 16);
		  }

		  if (!strcmp(strInstr, "DATA")){
			   Vector_Instruction = strtol(strOp1, NULL, 16);   	// Convert  Opcode 1 as hex
			   Write_Instruction_HEX_Data(Vector_PC << 1, Vector_Instruction);

			   if (Token_Count > 1){        // If a two word data define then
				   Vector_PC++;
				   Vector_Extra_Data = strtol(strOp2, NULL, 16);        // Convert  Opcode 2 as hex
				   Write_Instruction_HEX_Data(Vector_PC << 1, Vector_Extra_Data);
			   }
		  }


		  if (!strcmp(strInstr, "VCTR")){
			   Vector_Instruction = (atoi(strOp2) & 0x1FFF);
			   Write_Instruction_HEX_Data(Vector_PC << 1, Vector_Instruction);

			   Vector_PC++;
			   Vector_Extra_Data = (atoi(strOp1) & 0x1FFF) | ((atoi(strOp3) & 0x07) << 13);
			   Write_Instruction_HEX_Data(Vector_PC << 1, Vector_Extra_Data);
		  }
		  if (!strcmp(strInstr, "HALT")){
			   Vector_Instruction = 0x2020;
			   Write_Instruction_HEX_Data(Vector_PC << 1, Vector_Instruction);
		  }
		  if (!strcmp(strInstr, "SVEC")){
			   Vector_Instruction = 0x4000 | (atoi(strOp1) & 0x1F) | ((atoi(strOp2) & 0x1F) << 8) | ((atoi(strOp3) & 0x7) << 5);
			   Write_Instruction_HEX_Data(Vector_PC << 1, Vector_Instruction);
		  }
		  if (!strcmp(strInstr, "STAT")){
				if(dlgGame_Select->rgVectorType -> ItemIndex == 0){            // If Star Wars type
						Vector_Instruction = 0x6000 | ((atoi(strOp1) & 0x0F) << 8) | ((atoi(strOp2) & 0xFF));
                }
				if(dlgGame_Select->rgVectorType -> ItemIndex == 1){            // If Battlezone type
						Vector_Instruction = 0x6000 | ((atoi(strOp2) & 0x0F) << 4) | (atoi(strOp1) & 0x0F) | ((atoi(strOp3) & 0x0F) << 8);
				}
				if(dlgGame_Select->rgVectorType -> ItemIndex == 2){            // If Space Duel type
						Vector_Instruction = 0x6000 | ((atoi(strOp2) & 0x0F) << 4) | (atoi(strOp1) & 0x0F) | ((atoi(strOp3) & 0x0F) << 8);
//						Vector_Instruction = 0x6000 | (atoi(strOp1) & 0x0F) | ((atoi(strOp2) & 0xF) << 4);
				}

			   Write_Instruction_HEX_Data(Vector_PC << 1, Vector_Instruction);
		  }
		  if (!strcmp(strInstr, "SCAL")){
			   Vector_Instruction = 0x7000 | ((atoi(strOp1) & 0x07) << 8) | ((atoi(strOp2) & 0xFF));
			   Write_Instruction_HEX_Data(Vector_PC << 1, Vector_Instruction);
          }
          if (!strcmp(strInstr, "CNTR")){
			   Vector_Instruction = 0x8000 | (atoi(strOp1) & 0xFF);
			   Write_Instruction_HEX_Data(Vector_PC << 1, Vector_Instruction);
          }
		  if (!strcmp(strInstr, "JRSL")){
			   if (Pass == 1){
					strcpy(Temp_Label.strLabel, strOp1);
					Label_Location = Vector_Address_Label2.find(Temp_Label);
					if(Label_Location == Vector_Address_Label2.end()) { // If label doesn't exist, probably false instruction trying to jump to middle of a vector instruction
						sscanf(strOp1, "L_%04X", &Invalid_Label_Address);
						Vector_Instruction = 0xA000 | (Invalid_Label_Address & 0x1FFF);
					}else{
						Vector_Instruction = 0xA000 | ((Label_Location->Address) & 0x1FFF);
					}

					Write_Instruction_HEX_Data(Vector_PC << 1, Vector_Instruction);
			   }
		  }
		  if (!strcmp(strInstr, "RTSL")){
			   Vector_Instruction = 0xC000;
			   Write_Instruction_HEX_Data(Vector_PC << 1, Vector_Instruction);
		  }
		  if (!strcmp(strInstr, "JMPL")){
			   if (Pass == 1){
					strcpy(Temp_Label.strLabel, strOp1);
					Label_Location = Vector_Address_Label2.find(Temp_Label);
					if(Label_Location == Vector_Address_Label2.end()) { // If label doesn't exist, probably false instruction trying to jump to middle of a vector instruction
						sscanf(strOp1, "L_%04X", &Invalid_Label_Address);
						Vector_Instruction = 0xE000 | (Invalid_Label_Address & 0x1FFF);
					}else{
						Vector_Instruction = 0xE000 | ((Label_Location->Address) & 0x1FFF);
					}



					Write_Instruction_HEX_Data(Vector_PC << 1, Vector_Instruction);
               }
		  }
		  Vector_PC++;
		}
		if (Assembly_Error || (Vector_PC > 0x4000)){
			break;
		}
	 }
	if (Assembly_Error || (Vector_PC > 0x4000)){
		break;
	}
 }


/* strcpy(Temp_Label.strLabel, "L_0000");
 Temp_Label.Address = 0x0000;
 Vector_Address_Label2.insert(Temp_Label);
 strcpy(Temp_Label.strLabel, "L_0001");
 Temp_Label.Address = 0x0001;
 Vector_Address_Label2.insert(Temp_Label);

 Label_Location = Vector_Address_Label2.find(Temp_Label);
 */

/*  for ( Label_Location=Vector_Address_Label2.begin() ; Label_Location != Vector_Address_Label2.end(); Label_Location++ ){
		Memo1 -> Lines -> Add(Label_Location->strLabel);
        Memo1 -> Lines -> Add(Label_Location->Address);
  }*/

}

//---------------------------------------------------------------------------

WORD __fastcall TForm1::Twos_to_Sign_Mag(int Twos_Complement_Value)
{
 WORD Sign_Magnitude;

 Sign_Magnitude = abs(Twos_Complement_Value) & 0x03FF;
 if(Twos_Complement_Value < 0){   	// If negative then
	Sign_Magnitude |= 0x0400;   	// Set sign bit
 }
 return Sign_Magnitude;
}


//---------------------------------------------------------------------------

void __fastcall TForm1::Assemble_DVG(void)
{
 WORD Vector_PC;
 WORD Vector_Instruction;
 WORD Vector_Extra_Data;
 WORD Token_Count;
 WORD X_bits, Y_bits;
 int Count;
 tVecSymbolTable2 Temp_Label;
 std::set<tVecSymbolTable2>::iterator Label_Location;
 WORD Invalid_Label_Address;
 int Assembly_Source_Line;
 char strLabel[32];
 char strInstr[16];
 char strOp1[16];
 char strOp2[16];
 char strOp3[16];
 char strOp4[16];
 char * ptrToken;
 char strLine_Buffer[128];
 char Pass;


 Vector_Address_Label2.clear();               // Clear symbol table

 for (Pass = 0; Pass < 2; Pass++){
 Vector_PC = 0;
 for (Assembly_Source_Line = 0;Assembly_Source_Line < Memo1 -> Lines->Count;Assembly_Source_Line++){                      // For each line
	lbAssembly_Line->Caption = Memo1->Lines->Strings[Assembly_Source_Line];//Assembly_Source_Line;
	 strcpy(strLine_Buffer, AnsiString(Memo1 -> Lines->Strings[Assembly_Source_Line]).c_str());
	 strcpy(strInstr, "");             // Flag that no instrucion
	 if ((strLine_Buffer[0] != '\t') && (strLine_Buffer[0] != ' ') && (strLine_Buffer[0] != '\n') && (strLine_Buffer[0] != '\r')){                        // If a label detected then
			   if((ptrToken = strtok(strLine_Buffer, ": \t\n\r")) != NULL){
					strcpy(strLabel, ptrToken);             // Copy label string

					if((ptrToken = strtok(NULL, " \t\n\r;")) != NULL){
						 strcpy(strInstr, ptrToken);             // Copy instruction string
						 Token_Count = 0;
					}else{
						 strcpy(strInstr, "");             // Flag that no instrucion
					}
					if((ptrToken = strtok(NULL, ", \t\n\r;")) != NULL){
						 strcpy(strOp1, ptrToken);             // Copy Operand 1 string
						 Token_Count = 1;
					}
					if((ptrToken = strtok(NULL, ", \t\n\r;")) != NULL){
						 strcpy(strOp2, ptrToken);             // Copy Operand 2 string
						 Token_Count = 2;
					}
					if((ptrToken = strtok(NULL, ", \t\n\r;")) != NULL){
						 strcpy(strOp3, ptrToken);             // Copy Operand 3 string
						 Token_Count = 3;
					}
					if((ptrToken = strtok(NULL, " \t\n\r;")) != NULL){
						 strcpy(strOp4, ptrToken);             // Copy Operand 4 string
						 Token_Count = 4;
					}
					if (Pass == 0){
						 strcpy(Temp_Label.strLabel, strLabel);
						 Temp_Label.Address = Vector_PC;
						 Vector_Address_Label2.insert(Temp_Label);
					}
			   }
	 }else{
					if((ptrToken = strtok(strLine_Buffer, " \t\n\r")) != NULL){
						 strcpy(strInstr, ptrToken);             // Copy instruction string
						 Token_Count = 0;
					}else{
						 strcpy(strInstr, "");             // Flag that no instrucion
					}
					if((ptrToken = strtok(NULL, ", \t\n\r;")) != NULL){
						 strcpy(strOp1, ptrToken);             // Copy Operand 1 string
						 Token_Count = 1;
					}
					if((ptrToken = strtok(NULL, ", \t\n\r;")) != NULL){
						 strcpy(strOp2, ptrToken);             // Copy Operand 2 string
						 Token_Count = 2;
					}
					if((ptrToken = strtok(NULL, ", \t\n\r;")) != NULL){
						 strcpy(strOp3, ptrToken);             // Copy Operand 3 string
						 Token_Count = 3;
					}
					if((ptrToken = strtok(NULL, " \t\n\r;")) != NULL){
						 strcpy(strOp4, ptrToken);             // Copy Operand 4 string
						 Token_Count = 4;
					}
	 }

	 if (strInstr[0] != 0){          // If instruction detected then
		  if (!strcmp(strInstr, "ORG")){          // Change vector address PC
			   Vector_PC = strtol(strOp1, NULL, 16);
		  }

		  if (!strcmp(strInstr, "DATA")){
			   Vector_Instruction = strtol(strOp1, NULL, 16);   	// Convert  Opcode 1 as hex
			   Write_Instruction_HEX_Data(Vector_PC << 1, Vector_Instruction);

			   if (Token_Count > 1){
				   Vector_PC++;
				   Vector_Extra_Data = strtol(strOp2, NULL, 16);        // Convert  Opcode 2 as hex
				   Write_Instruction_HEX_Data(Vector_PC << 1, Vector_Extra_Data);
			   }
		  }

		  if (!strcmp(strInstr, "VCTR0")){
			   Vector_Instruction = Twos_to_Sign_Mag(atoi(strOp2));
			   Write_Instruction_HEX_Data(Vector_PC << 1, Vector_Instruction);

			   Vector_PC++;
			   Vector_Extra_Data = Twos_to_Sign_Mag(atoi(strOp1)) | ((atoi(strOp3) & 0x0F) << 12);
			   Write_Instruction_HEX_Data(Vector_PC << 1, Vector_Extra_Data);
		  }
		  if (!strcmp(strInstr, "VCTR1")){
			   Vector_Instruction = 0x1000 | Twos_to_Sign_Mag(atoi(strOp2));
			   Write_Instruction_HEX_Data(Vector_PC << 1, Vector_Instruction);

			   Vector_PC++;
			   Vector_Extra_Data = Twos_to_Sign_Mag(atoi(strOp1)) | ((atoi(strOp3) & 0x0F) << 12);
			   Write_Instruction_HEX_Data(Vector_PC << 1, Vector_Extra_Data);
		  }
		  if (!strcmp(strInstr, "VCTR2")){
			   Vector_Instruction = 0x2000 | Twos_to_Sign_Mag(atoi(strOp2));
			   Write_Instruction_HEX_Data(Vector_PC << 1, Vector_Instruction);

			   Vector_PC++;
			   Vector_Extra_Data = Twos_to_Sign_Mag(atoi(strOp1)) | ((atoi(strOp3) & 0x0F) << 12);
			   Write_Instruction_HEX_Data(Vector_PC << 1, Vector_Extra_Data);
		  }
		  if (!strcmp(strInstr, "VCTR3")){
			   Vector_Instruction = 0x3000 | Twos_to_Sign_Mag(atoi(strOp2));
			   Write_Instruction_HEX_Data(Vector_PC << 1, Vector_Instruction);

			   Vector_PC++;
			   Vector_Extra_Data = Twos_to_Sign_Mag(atoi(strOp1)) | ((atoi(strOp3) & 0x0F) << 12);
			   Write_Instruction_HEX_Data(Vector_PC << 1, Vector_Extra_Data);
		  }
		  if (!strcmp(strInstr, "VCTR4")){
			   Vector_Instruction = 0x4000 | Twos_to_Sign_Mag(atoi(strOp2));
			   Write_Instruction_HEX_Data(Vector_PC << 1, Vector_Instruction);

			   Vector_PC++;
			   Vector_Extra_Data = Twos_to_Sign_Mag(atoi(strOp1)) | ((atoi(strOp3) & 0x0F) << 12);
			   Write_Instruction_HEX_Data(Vector_PC << 1, Vector_Extra_Data);
		  }
		  if (!strcmp(strInstr, "VCTR5")){
			   Vector_Instruction = 0x5000 | Twos_to_Sign_Mag(atoi(strOp2));
			   Write_Instruction_HEX_Data(Vector_PC << 1, Vector_Instruction);

			   Vector_PC++;
			   Vector_Extra_Data = Twos_to_Sign_Mag(atoi(strOp1)) | ((atoi(strOp3) & 0x0F) << 12);
			   Write_Instruction_HEX_Data(Vector_PC << 1, Vector_Extra_Data);
		  }
		  if (!strcmp(strInstr, "VCTR6")){
			   Vector_Instruction = 0x6000 | Twos_to_Sign_Mag(atoi(strOp2));
			   Write_Instruction_HEX_Data(Vector_PC << 1, Vector_Instruction);

			   Vector_PC++;
			   Vector_Extra_Data = Twos_to_Sign_Mag(atoi(strOp1)) | ((atoi(strOp3) & 0x0F) << 12);
			   Write_Instruction_HEX_Data(Vector_PC << 1, Vector_Extra_Data);
		  }
		  if (!strcmp(strInstr, "VCTR7")){
			   Vector_Instruction = 0x7000 | Twos_to_Sign_Mag(atoi(strOp2));
			   Write_Instruction_HEX_Data(Vector_PC << 1, Vector_Instruction);

			   Vector_PC++;
			   Vector_Extra_Data = Twos_to_Sign_Mag(atoi(strOp1)) | ((atoi(strOp3) & 0x0F) << 12);
			   Write_Instruction_HEX_Data(Vector_PC << 1, Vector_Extra_Data);
		  }
		  if (!strcmp(strInstr, "VCTR8")){
			   Vector_Instruction = 0x8000 | Twos_to_Sign_Mag(atoi(strOp2));
			   Write_Instruction_HEX_Data(Vector_PC << 1, Vector_Instruction);

			   Vector_PC++;
			   Vector_Extra_Data = Twos_to_Sign_Mag(atoi(strOp1)) | ((atoi(strOp3) & 0x0F) << 12);
			   Write_Instruction_HEX_Data(Vector_PC << 1, Vector_Extra_Data);
		  }
		  if (!strcmp(strInstr, "VCTR9")){
			   Vector_Instruction = 0x9000 | Twos_to_Sign_Mag(atoi(strOp2));
			   Write_Instruction_HEX_Data(Vector_PC << 1, Vector_Instruction);

			   Vector_PC++;
			   Vector_Extra_Data = Twos_to_Sign_Mag(atoi(strOp1)) | ((atoi(strOp3) & 0x0F) << 12);
			   Write_Instruction_HEX_Data(Vector_PC << 1, Vector_Extra_Data);
		  }


		  if (!strcmp(strInstr, "HALT")){
			   Vector_Instruction = 0xB0B0;     // HALT is repeated in low byte
			   Write_Instruction_HEX_Data(Vector_PC << 1, Vector_Instruction);
		  }
		  if (!strcmp(strInstr, "SVEC")){
				X_bits = abs(atoi(strOp1)) & 0x0003;
				if(atoi(strOp1) < 0){   // If negative then
					X_bits |= 0x0004;   // Set sign bit
				}
				Y_bits = abs(atoi(strOp2)) & 0x0003;
				if(atoi(strOp2) < 0){   // If negative then
					Y_bits |= 0x0004;   // Set sign bit
				}
//			   Vector_Instruction = 0xF000 | (atoi(strOp2) & 0x07) | ((atoi(strOp1) & 0x07) << 8) | ((atoi(strOp3) & 0xF) << 4) | ((atoi(strOp4) & 0x2) << 2) | ((atoi(strOp4) & 0x1) << 11);
			   Vector_Instruction = 0xF000 | X_bits | (Y_bits << 8) | ((atoi(strOp3) & 0xF) << 4) | ((atoi(strOp4) & 0x2) << 2) | ((atoi(strOp4) & 0x1) << 11);
			   Write_Instruction_HEX_Data(Vector_PC << 1, Vector_Instruction);
		  }
		  if (!strcmp(strInstr, "LABS")){
			   Vector_Instruction = 0xA000 | Twos_to_Sign_Mag(atoi(strOp2));
			   Write_Instruction_HEX_Data(Vector_PC << 1, Vector_Instruction);

			   Vector_PC++;
			   Vector_Extra_Data = Twos_to_Sign_Mag(atoi(strOp1)) | ((atoi(strOp3) & 0x0F) << 12);
			   Write_Instruction_HEX_Data(Vector_PC << 1, Vector_Extra_Data);
		  }
		  if (!strcmp(strInstr, "JRSL")){
			   if (Pass == 1){
					strcpy(Temp_Label.strLabel, strOp1);
					Label_Location = Vector_Address_Label2.find(Temp_Label);
					if(Label_Location == Vector_Address_Label2.end()) { // If label doesn't exist, probably false instruction trying to jump to middle of a vector instruction
						sscanf(strOp1, "L_%04X", &Invalid_Label_Address);
						Vector_Instruction = 0xC000 | (Invalid_Label_Address & 0x0FFF);
					}else{
						Vector_Instruction = 0xC000 | ((Label_Location->Address) & 0x0FFF);
					}
					Write_Instruction_HEX_Data(Vector_PC << 1, Vector_Instruction);
			   }
		  }
		  if (!strcmp(strInstr, "RTSL")){
			   Vector_Instruction = 0xD000;
			   Write_Instruction_HEX_Data(Vector_PC << 1, Vector_Instruction);
		  }
		  if (!strcmp(strInstr, "JMPL")){
			   if (Pass == 1){
					strcpy(Temp_Label.strLabel, strOp1);
					Label_Location = Vector_Address_Label2.find(Temp_Label);
					if(Label_Location == Vector_Address_Label2.end()) { // If label doesn't exist, probably false instruction trying to jump to middle of a vector instruction
						sscanf(strOp1, "L_%04X", &Invalid_Label_Address);
						Vector_Instruction = 0xE000 | (Invalid_Label_Address & 0x0FFF);
					}else{
						Vector_Instruction = 0xE000 | ((Label_Location->Address) & 0xFFF);
					}
					Write_Instruction_HEX_Data(Vector_PC << 1, Vector_Instruction);
			   }
		  }
		  Vector_PC++;      // Increment address counter ready for next instruction location
		}
	if (Assembly_Error || (Vector_PC > 0x2000)){
			break;
		}
	 }
	if (Assembly_Error || (Vector_PC > 0x2000)){
		break;
	}
 }


/* strcpy(Temp_Label.strLabel, "L_0000");
 Temp_Label.Address = 0x0000;
 Vector_Address_Label2.insert(Temp_Label);
 strcpy(Temp_Label.strLabel, "L_0001");
 Temp_Label.Address = 0x0001;
 Vector_Address_Label2.insert(Temp_Label);

 Label_Location = Vector_Address_Label2.find(Temp_Label);
 */

/*  for ( Label_Location=Vector_Address_Label2.begin() ; Label_Location != Vector_Address_Label2.end(); Label_Location++ ){
		Memo1 -> Lines -> Add(Label_Location->strLabel);
		Memo1 -> Lines -> Add(Label_Location->Address);
  }*/

}


void __fastcall TForm1::Button5Click(TObject *Sender)
{
 Assembly_Error = FALSE;
 lbAssembly_Line->Caption = "";
 lbAssemble_Error->Caption = "";
 if (dlgGame_Select->cbAVG_Selected -> Checked == TRUE){
		Assemble_AVG();
 }else{
		Assemble_DVG();
 }
 Draw_Hex_Grid();
 bnDraw_ButtonClick(Form1);
}
//---------------------------------------------------------------------------



void __fastcall TForm1::btConvert_PLTClick(TObject *Sender)
{
 HANDLE hRead_File;
 DWORD Bytes_Read;
 char * PLT_String = (char*)malloc(2*1024*1024);
 int pltX;
 int pltY;
 int Pen;
 short Rel_X = 512;
 short Rel_Y = 384;
 short Last_Rel_X = 512;
 short Last_Rel_Y = 384;
 char pltInstr[8];
 char Decode[64];
 char strRelative[64];
 char * token;

 if(OpenDialog3 -> Execute()){
	 Label6 -> SetTextBuf(OpenDialog3 -> FileName.c_str());		// Show file name
	 if ((hRead_File = CreateFile(AnsiString(OpenDialog3 -> FileName).c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE) {

		ReadFile(hRead_File, PLT_String, 65536, &Bytes_Read, NULL);
		CloseHandle(hRead_File);
		token = strtok(PLT_String, "IN;");

				Memo1 -> Clear();                 // Clear Memo form
				sprintf(strRelative, "\tCNTR\t64");
				Memo1 -> Lines -> Add(strRelative);
				sprintf(strRelative, "\tSCAL\t1, 128");
				Memo1 -> Lines -> Add(strRelative);
				sprintf(strRelative, "\tSTAT\t7, 128");
				Memo1 -> Lines -> Add(strRelative);

		do {
				token = strtok(NULL, ";");
				if (*token == NULL) {
					break;
				}
						pltInstr[0] = token[0];
						pltInstr[1] = token[1];
						pltInstr[2] = 0;

						if (!strcmp(pltInstr, "SP")){
								sscanf(&token[2], "%d", &Pen);
								sprintf(Decode, "%s X:%d Y:%d", pltInstr, pltX, pltY);

						}
						if (!strcmp(pltInstr, "PU")){
								if (strlen(token) > 2){
										sscanf(&token[2], "%d,%d", &pltX, &pltY);
										sprintf(Decode, "%s X:%d Y:%d", pltInstr, pltX, pltY);

										Rel_X = Last_Rel_X - (short)pltX;
										Rel_Y = Last_Rel_Y - (short)pltY;
										Last_Rel_X = (short)pltX;
										Last_Rel_Y = (short)pltY;

								if ((Rel_X <= 15) && (Rel_X >= -15) && (Rel_Y <= 15) && (Rel_Y >= -15) && ((Rel_X != 0) && (Rel_Y != 0))) {
//									sprintf(strRelative, "\tVCTR\t%d, %d, 0", -Rel_X, Rel_Y);
									sprintf(strRelative, "\tSVEC\t%d, %d, 0", -Rel_X, Rel_Y);
								}else{
									sprintf(strRelative, "\tVCTR\t%d, %d, 0", -Rel_X, Rel_Y);
								}

								Memo1 -> Lines -> Add(strRelative);

								}else{
							sprintf(Decode, "%s", pltInstr);
										break;
								}
						}
						if (!strcmp(pltInstr, "PD")){
								if (strlen(token) > 2){
										sscanf(&token[2], "%d,%d", &pltX, &pltY);
										sprintf(Decode, "%s X:%d Y:%d", pltInstr, pltX, pltY);
										Rel_X = Last_Rel_X - (short)pltX;
										Rel_Y = Last_Rel_Y - (short)pltY;
										Last_Rel_X = (short)pltX;
										Last_Rel_Y = (short)pltY;
									if ((Rel_X <= 15) && (Rel_X >= -15) && (Rel_Y <= 15) && (Rel_Y >= -15) && ((Rel_X != 0) && (Rel_Y != 0))) {
//										sprintf(strRelative, "\tVCTR\t%d, %d, 7", -Rel_X, Rel_Y);
										sprintf(strRelative, "\tSVEC\t%d, %d, 7", -Rel_X, Rel_Y);
									}else{
										sprintf(strRelative, "\tVCTR\t%d, %d, 7", -Rel_X, Rel_Y);
									}
									Memo1 -> Lines -> Add(strRelative);
								}else{
									sprintf(Decode, "%s", pltInstr);
								}
						}
//        		Memo1 -> Lines -> Add(Decode);



		}while(1);
		sprintf(strRelative, "\tHALT");
		Memo1 -> Lines -> Add(strRelative);
	 }
  }
 free(PLT_String);
}
//---------------------------------------------------------------------------










void __fastcall TForm1::btChange_GameClick(TObject *Sender)
{
 dlgGame_Select->ShowModal();
 if(dlgGame_Select->rgVectorType->ItemIndex == 3){
	sgHex_Data->RowCount = 257;
	btConvert_PLT ->Enabled = FALSE;
 }else{
	sgHex_Data->RowCount = 513;
	btConvert_PLT ->Enabled = TRUE;
 }
}
//---------------------------------------------------------------------------







