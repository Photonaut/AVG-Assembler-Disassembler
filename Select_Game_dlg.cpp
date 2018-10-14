//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Select_Game_dlg.h"
//---------------------------------------------------------------------
#pragma resource "*.dfm"
TdlgGame_Select *dlgGame_Select;
//--------------------------------------------------------------------- 
__fastcall TdlgGame_Select::TdlgGame_Select(TComponent* AOwner)
	: TForm(AOwner)
{
}
//---------------------------------------------------------------------
void __fastcall TdlgGame_Select::rgVectorTypeClick(TObject *Sender)
{
 if(rgVectorType -> ItemIndex == 0){            // Star Wars type
		cbBig_Endian_6809 -> Checked = TRUE;
		cbAVG_Selected -> Checked = TRUE;
 }
 if(rgVectorType -> ItemIndex == 1){            // Battlezone type
		cbBig_Endian_6809 -> Checked = FALSE;
		cbAVG_Selected -> Checked = TRUE;
 }
 if(rgVectorType -> ItemIndex == 2){            // Space Duel type
		cbBig_Endian_6809 -> Checked = FALSE;
		cbAVG_Selected -> Checked = TRUE;
 }
 if(rgVectorType -> ItemIndex == 3){            // Asteroids DVG type
		cbBig_Endian_6809 -> Checked = FALSE;
		cbAVG_Selected -> Checked = FALSE;
 }

}
//---------------------------------------------------------------------------


void __fastcall TdlgGame_Select::Timer1Timer(TObject *Sender)
{
 static int Position = 0;
 static WORD Direction = 0;


 if(Direction){
	Position += 1;
	if(Position > 7){
		Direction = 0;
	}
 }else{
	Position -= 1;
	if(Position < -7){
		Direction = 1;
	}
 }
 Image1->Top = 24 + Position;

}
//---------------------------------------------------------------------------

