#ifndef TEXTGENERATOR_H
#define TEXTGENERATOR_H

#include <string>
#include "../../../tools/quad2.h"

class Graphics;
class Texture;

///<summary>Simple text generator</summary>
class TextGenerator{
public:
	///<summary>Creates generated text data</summary>
	///<param name="text">Text</param>
	///<param name="size">Text size</param>
	///<param name="align">Align</param>
	///<param name="buffering">Buffering</param>
	TextGenerator(const std::string& text,int size=1,int align=0,bool buffering=true);
	~TextGenerator();
	///<summary>Updates generated text with current text</summary>
	///<param name="text">Text</param>
	void SetText(const std::string& text);

	///<summary>Font sizes</summary>
	enum FONTS{
		FONT_NORMAL = 1,
		FONT_BIG = 2,
		FONT_DIVIDER = 20,
	};
	///<summary>Font aligns</summary>
	enum ALIGNS{
		ALIGN_LEFT_TOP=0,
		ALIGN_LEFT_CENTER,
		ALIGN_LEFT_BOTTOM,
		ALIGN_CENTER_TOP,
		ALIGN_CENTER_CENTER,
		ALIGN_CENTER_BOTTOM,
		ALIGN_RIGHT_TOP,
		ALIGN_RIGHT_CENTER,
		ALIGN_RIGHT_BOTTOM,
	};
	///<summary>Size of generated texture</summary>
	quad2 geometry;
	Texture* texture;
private:
	int pixels;
	bool buffering;
};

#endif