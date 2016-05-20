#ifndef INPUT_H
#define INPUT_H

#include "../ClientComponent.h"
#include <vector>

class Input : public ClientComponent {
private:
	Input();
	friend Input *g_Input();

public:
	~Input() override;
	void Tick() override;
	std::vector<char> keyboard;
	std::vector<char> mouse;
	bool quit;
	int mouseX;
	int mouseY;
	int mouseWheel;
};
Input *g_Input();

#endif
