/************************************************************
************************************************************/
#include <sj_common.h>

/************************************************************
************************************************************/
/********************
********************/
int GPIO_0 = 0;
int GPIO_1 = 0;

const float _PI = 3.1415;

/********************
********************/
GUI_GLOBAL* Gui_Global = NULL;

FILE* fp_Log = nullptr;


/************************************************************
func
************************************************************/
/******************************
******************************/
double LPF(double LastVal, double NewVal, double Alpha_dt, double dt)
{
	double Alpha;
	if((Alpha_dt <= 0) || (Alpha_dt < dt))	Alpha = 1;
	else									Alpha = 1/Alpha_dt * dt;
	
	return NewVal * Alpha + LastVal * (1 - Alpha);
}

/******************************
******************************/
double LPF(double LastVal, double NewVal, double Alpha)
{
	if(Alpha < 0)		Alpha = 0;
	else if(1 < Alpha)	Alpha = 1;
	
	return NewVal * Alpha + LastVal * (1 - Alpha);
}

/******************************
******************************/
bool checkIf_ContentsExist(char* ret, char* buf)
{
	if( (ret == NULL) || (buf == NULL)) return false;
	
	string str_Line = buf;
	Align_StringOfData(str_Line);
	vector<string> str_vals = ofSplitString(str_Line, ",");
	if( (str_vals.size() == 0) || (str_vals[0] == "") ){ // no_data or exist text but it's",,,,,,,".
		return false;
	}else{
		return true;
	}
}

/******************************
******************************/
void Align_StringOfData(string& s)
{
	size_t pos;
	while((pos = s.find_first_of(" 　\t\n\r")) != string::npos){ // 半角・全角space, \t 改行 削除
		s.erase(pos, 1);
	}
}

/******************************
******************************/
void print_separatoin()
{
	printf("---------------------------------\n");
}

/******************************
******************************/
void ClearFbo(ofFbo& fbo)
{
	fbo.begin();
		ofClear(0, 0, 0, 255);
	fbo.end();
}

/******************************
******************************/
float toRad(float val){
	return val * _PI / 180.0;
}

/******************************
******************************/
float toDeg(float val){
	return val * 180.0 / _PI;
}

/************************************************************
class
************************************************************/

/******************************
******************************/
void GUI_GLOBAL::setup(string GuiName, string FileName, float x, float y)
{
	/********************
	********************/
	gui.setup(GuiName.c_str(), FileName.c_str(), x, y);
	
	/********************
	********************/
	Group_shader.setup("shader");
		Group_shader.add(shader_xX.setup("shader_xX", 1.0, 0.5, 2.0));
	gui.add(&Group_shader);
	
	/********************
	********************/
	gui.minimizeAll();
}

