#pragma once

sf::Text heading, currentIterations, hudFrames, colorMeth, controls, instructions1, instructions2, mpfrHeading, hudMpfr;
sf::Font font;
float frames;	  // To store FramesPerSecond
int hudCount = 0; // To count loops for hud
bool showControls = false;
bool showMPFRHud = false;

template <typename T>
//function to convert a non-string variable to a string.
std::string to_string(T value)
{
	std::ostringstream os;
	os << value;
	return os.str();
}

void InitHud();
void DrawHud(sf::RenderWindow* win);

void InitHud()
{
	currentIterations.setFont(font);
	currentIterations.setFillColor(sf::Color::White);
	currentIterations.setCharacterSize(10);
	currentIterations.setPosition(80, 5);
	currentIterations.setString(to_string(128));
	heading = currentIterations;
	hudFrames = currentIterations;
	colorMeth = currentIterations;
	controls = currentIterations;
	mpfrHeading = currentIterations;
	hudMpfr = currentIterations;
	instructions1 = currentIterations;
	instructions1.setCharacterSize(15);
	instructions2 = instructions1;

	heading.setPosition(5, 5);
	heading.setString("ITERATIONS-\n\nFRAMES-\n\nCOLOR METHOD-");
	hudFrames.setPosition(80,23);
	hudFrames.setString("0");
	colorMeth.setString("SINGLE");
	colorMeth.setPosition(80,41);
	mpfrHeading.setPosition(5,61);
	mpfrHeading.setString("MPFR PRECISION-");
	hudMpfr.setPosition(80,61);
	hudMpfr.setString("80");
	controls.setString("PRESS I TO SHOW CONTROLS");
	controls.setPosition(5,WIN_HEIGHT-15);

	instructions1.setPosition(WIN_WIDTH/2,0);
	instructions2.setPosition((WIN_WIDTH/2)+150,0);
	instructions1.setString(
	"SET RGB VALUES FOR SINGLE COLOR METHOD\nR and T-\nG and H-\nB and N-\n\nC-\nA- \n\nX- \nHOME and END- \nINS and DEL- \n\n, and .-\nPAGEUP and PAGEDOWN-\nARROW KEYS-\nLMB and RMB-\nMMB-\nS-\nV-\nF1-\nF2-\nF3-\nTILDE-\nF11 and F12-");
	instructions2.setString(
	"\nRED\nGREEN\nBLUE\n\nCHANGE COLOR METHOD\nANIMATE COLOR\n\nNORMAL MAP\nLIGHT HEIGHT\nLIGHT ANGLE\n\nINTERIOR COLOR\nITERATIONS\nPAN\nZOOM\nRESET VIEW\nTAKE SCREENSHOT\nTAKE VIDEO FRAMES\nAUTOZOOM\nAUTO ITERATIONS\nRESET Y OFFSET\nMPFR ON/OFF\nMPFR PRECISION");
	//load the font
	//note that this path is relative to the workspace
	if (!font.loadFromFile("content/shredded.ttf"))
	{
		std::cout << "Failed to load font" << std::endl;
	}
}

void DrawHud(sf::RenderWindow* win)
{
	win->draw(heading);
	win->draw(currentIterations);
	win->draw(hudFrames);
	win->draw(colorMeth);
	win->draw(controls);
	if(showMPFRHud){
		win->draw(mpfrHeading);
		win->draw(hudMpfr);
	}
	if(showControls){
		win->draw(instructions1);
		win->draw(instructions2);
	}
}
