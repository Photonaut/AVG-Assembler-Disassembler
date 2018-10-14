//---------------------------------------------------------------------------

#ifndef DAVGAD_MainH
#define DAVGAD_MainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
#include <ComCtrls.hpp>
#include <Graphics.hpp>
#include <Buttons.hpp>
#include <Vcl.Grids.hpp>
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------

class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TOpenDialog *OpenDialog1;
        TSaveDialog *SaveDialog1;
        TLabel *Label1;
        TTimer *Timer1;
     TLabel *lbConnected;
     TLabel *lbID_String;
     TPageControl *PageControl1;
     TTabSheet *Logs;
     TLabel *Label43;
     TMemo *Memo1;
     TButton *Button1;
	 TButton *Button2;
        TImage *Image1;
	TBitBtn *bnDraw_Button;
        TEdit *Edit1;
     TCheckBox *CheckBox1;
        TLabel *Label2;
     TLabel *Label3;
     TSaveDialog *SaveDialog2;
     TOpenDialog *OpenDialog2;
	 TButton *Button5;
     TLabel *Label4;
     TLabel *Label5;
     TTabSheet *TabSheet1;
     TButton *Button3;
     TButton *Button4;
        TOpenDialog *OpenDialog3;
        TLabel *Label6;
	TButton *btConvert_PLT;
	TProgressBar *pbLoad_Progress;
	TTabSheet *TabSheet2;
	TStringGrid *sgHex_Data;
	TLabel *Label7;
	TLabel *lbChecksum;
	TLabel *lbAssemble_Error;
	TLabel *lbAssembly_Line;
	TButton *btChange_Game;
	TCheckBox *cbError_Check;
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall FormShow(TObject *Sender);
     void __fastcall FormCreate(TObject *Sender);
        void __fastcall bnDraw_ButtonClick(TObject *Sender);
        void __fastcall Image1Click(TObject *Sender);
        void __fastcall Memo1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
     void __fastcall Button4Click(TObject *Sender);
     void __fastcall Button3Click(TObject *Sender);
     void __fastcall Image1MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
	 void __fastcall Button5Click(TObject *Sender);
		void __fastcall btConvert_PLTClick(TObject *Sender);

		WORD __fastcall Twos_to_Sign_Mag(int Twos_Complement_Value);
	void __fastcall btChange_GameClick(TObject *Sender);











private:	// User declarations
		bool Programmer_Mode;
        bool Assembly_Error;
        WORD Load_Progress_Value;

        void __fastcall Disassemble_AVG(void);
        void __fastcall Disassemble_DVG(void);
        void __fastcall Assemble_AVG(void);
        void __fastcall Assemble_DVG(void);
        void __fastcall Draw_AVG(void);
        void __fastcall Draw_DVG(void);
		void __fastcall Find_Jump_Addresses(void);
		void __fastcall Write_Instruction_HEX_Data(WORD Vector_PC, WORD Vector_Instruction);
		void __fastcall Draw_Hex_Grid(void);

public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
