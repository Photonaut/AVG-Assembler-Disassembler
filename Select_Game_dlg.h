//----------------------------------------------------------------------------
#ifndef Select_Game_dlgH
#define Select_Game_dlgH
//----------------------------------------------------------------------------
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Graphics.hpp>
#include <System.Classes.hpp>
#include <System.SysUtils.hpp>
#include <Winapi.Windows.hpp>
#include <System.hpp>
//----------------------------------------------------------------------------
class TdlgGame_Select : public TForm
{
__published:
	TButton *OKBtn;
	TButton *CancelBtn;
	TBevel *Bevel1;
	TRadioGroup *rgVectorType;
	TCheckBox *cbBig_Endian_6809;
	TCheckBox *cbAVG_Selected;
	TImage *Image1;
	TTimer *Timer1;
	void __fastcall rgVectorTypeClick(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
private:
public:
	virtual __fastcall TdlgGame_Select(TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern PACKAGE TdlgGame_Select *dlgGame_Select;
//----------------------------------------------------------------------------
#endif    
