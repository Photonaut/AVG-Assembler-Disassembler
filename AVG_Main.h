//---------------------------------------------------------------------------

#ifndef AVG_MainH
#define AVG_MainH
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
        TBitBtn *BitBtn1;
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
     TRadioGroup *rgVectorType;
     TCheckBox *CheckBox2;
     TCheckBox *CheckBox3;
     TButton *Button3;
     TButton *Button4;
        TOpenDialog *OpenDialog3;
        TLabel *Label6;
	TButton *Button6;
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall FormShow(TObject *Sender);
     void __fastcall FormCreate(TObject *Sender);
        void __fastcall BitBtn1Click(TObject *Sender);
        void __fastcall Image1Click(TObject *Sender);
        void __fastcall Memo1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
     void __fastcall Button4Click(TObject *Sender);
     void __fastcall Button3Click(TObject *Sender);
     void __fastcall Image1MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
     void __fastcall Button5Click(TObject *Sender);
        void __fastcall rgVectorTypeClick(TObject *Sender);
		void __fastcall Button6Click(TObject *Sender);


private:	// User declarations
        bool Programmer_Mode;

        void __fastcall Disassemble_AVG(void);
        void __fastcall Disassemble_DVG(void);
        void __fastcall Assemble_AVG(void);
        void __fastcall Assemble_DVG(void);
        void __fastcall Draw_AVG(void);
        void __fastcall Draw_DVG(void);
        void __fastcall Find_Jump_Addresses(void);
        void __fastcall Write_Instruction_HEX_Data(WORD Vector_PC, WORD Vector_Instruction);

public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
