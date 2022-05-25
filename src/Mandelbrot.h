#pragma once
#include "ColorConvert.h"
#include "Tools.h"
#include <complex>
#include <algorithm>
#include <gmp.h>
#include <mpfr.h>

sf::VertexArray vertexarrayPoints(sf::Points, MAX_NUM_PARTICLES); // To store calculated pixels
sf::Vector2i mousePos;											  // To store mouse position

constexpr int maxThreads = 8; // Max amount of threads to use for Fractal function

sf::Color interiorColor = sf::Color::Black; // Used for interior Coloring of mandelbrot
unsigned char interiorHue = 0;
HsvColor bhsv = { 255, 255, 100 };
RgbColor brgb;

sf::Clock colortimer;	// Use a timer for animated color method
bool animated = false;

float rAmount = 1.0f; 	// RGB Multipliers to change colors
float gAmount = 0.2f;
float bAmount = 0.2f;
int colorMethod = 0; 	// For Selecting method of colorization

double offsetX = 0.0; 			// Used for Panning
double offsetY = 0.0;
double zmx1 = WIN_WIDTH / 4; 	// Used for Zooming
double zmx2 = 2;
double zmy1 = WIN_HEIGHT / 4;
double zmy2 = 2;
double zmAmount = 1.1;

mpfr_t offsetX_T,offsetY_T,zmx1_T,zmx2_T,zmy1_T,zmy2_T, zmAmount_T, bail_T, t1_T; // MPFR copies of Pan and Zoom variables
bool useMPFR = false;			// To turn MPFR on/off
int mpfrPrecision = 80;			// MPFR bits of mantissa precision

bool zoomIn = false;
bool zoomOut = false;
bool autoZoomIn = false;
int maxiterations = 128;
bool autoIterations = false;		// Auto increment iterations during zoom
bool canCalculateFractal = true;	// For only calculating Fractal after a zoom or pan
bool burningShip = false;
const double escapeRadius = 4.0;
const double eps = 0.001;

// Normal Mapping variables
const double doublepi = 3.141592653589793238;
double lightHeight = 1.6; 	// Height Factor of incoming light
double angle = 45.0; 		// Angle of incoming light
const std::complex<double> complexi(0, 1);
std::complex<double> u;
std::complex<double> v = exp(complexi*angle*2.0*doublepi/360.0);
double t = 0.0;				// To store normal map lerp value
bool normalMap = false;

sf::Image image;	// To store image for From Image Color Method

enum fractalType_e {MANDELBROT, BURNINGSHIP, CELTIC, FRACTALTYPE_COUNT};
int fractalType = MANDELBROT;

void InitVertexArray();
void InitMPFR();
void SetMPFRPrecision();
void CalculateFractal(uint start, uint end);
void CalculateFractalMPFR(uint start, uint end);
void CreateFractalThreads();
void ResetView();
void ZoomIn(sf::Window& window);
void ZoomOut(sf::Window& window);
void MandelbrotFractal(double& a, double& b, double ca, double cb);
void BurningShipFractal(double& a, double& b, double ca, double cb);
void CelticFractal(double& a, double& b, double ca, double cb);

void InitVertexArray()
{
	for (uint y = 0; y < WIN_HEIGHT; y++)
	{
		for (uint x = 0; x < WIN_WIDTH; x++)
		{
			vertexarrayPoints[x + y * WIN_WIDTH].position.x = x;
			vertexarrayPoints[x + y * WIN_WIDTH].position.y = y;
		}
	}

	// Load an image file from a file
	if (!image.loadFromFile("content/image.jpg"))
    	std::cout << "image.jpg NOT FOUND";
}
void InitMPFR()
{
	mpfr_init2(offsetX_T, mpfrPrecision);
	mpfr_init2(offsetY_T, mpfrPrecision);
	mpfr_init2(zmx1_T, mpfrPrecision);
	mpfr_init2(zmx2_T, mpfrPrecision);
	mpfr_init2(zmy1_T, mpfrPrecision);
	mpfr_init2(zmy2_T, mpfrPrecision);
	mpfr_init2(zmAmount_T, mpfrPrecision);
	mpfr_init2(t1_T, mpfrPrecision);
	mpfr_init2(bail_T, mpfrPrecision);

	mpfr_set_d(offsetX_T, offsetX, GMP_RNDN);
	mpfr_set_d(offsetY_T, offsetY, GMP_RNDN);
	mpfr_set_d(zmx1_T, zmx1, GMP_RNDN);
	mpfr_set_d(zmx2_T, zmx2, GMP_RNDN);
	mpfr_set_d(zmy1_T, zmy1, GMP_RNDN);
	mpfr_set_d(zmy2_T, zmy2, GMP_RNDN);
	mpfr_set_d(zmAmount_T, zmAmount, GMP_RNDN);
	mpfr_set_d(t1_T, 0.0, GMP_RNDN);
	mpfr_set_d(bail_T, escapeRadius, GMP_RNDN);
}

void SetMPFRPrecision(){
	mpfr_t temp;
	mpfr_init2(temp, mpfrPrecision);

	mpfr_set(temp, offsetX_T, GMP_RNDN);
	mpfr_set_prec(offsetX_T,  	mpfrPrecision);
	mpfr_set(offsetX_T, temp, GMP_RNDN);

	mpfr_set(temp, offsetY_T, GMP_RNDN);
	mpfr_set_prec(offsetY_T,  	mpfrPrecision);
	mpfr_set(offsetY_T, temp, GMP_RNDN);

	mpfr_set(temp, zmx1_T, GMP_RNDN);
	mpfr_set_prec(zmx1_T, 		mpfrPrecision);
	mpfr_set(zmx1_T, temp, GMP_RNDN);

	mpfr_set(temp, zmx2_T, GMP_RNDN);
	mpfr_set_prec(zmx2_T, 		mpfrPrecision);
	mpfr_set(zmx2_T, temp, GMP_RNDN);

	mpfr_set(temp, zmy1_T, GMP_RNDN);
	mpfr_set_prec(zmy1_T, 		mpfrPrecision);
	mpfr_set(zmy1_T, temp, GMP_RNDN);

	mpfr_set(temp, zmy2_T, GMP_RNDN);
	mpfr_set_prec(zmy2_T, 		mpfrPrecision);
	mpfr_set(zmy2_T, temp, GMP_RNDN);

	mpfr_set(temp, zmAmount_T, GMP_RNDN);
	mpfr_set_prec(zmAmount_T,	mpfrPrecision);
	mpfr_set(zmAmount_T, temp, GMP_RNDN);

	mpfr_set(temp, t1_T, GMP_RNDN);
	mpfr_set_prec(t1_T, 		mpfrPrecision);
	mpfr_set(t1_T, temp, GMP_RNDN);

	mpfr_set(temp, bail_T, GMP_RNDN);
	mpfr_set_prec(bail_T, 		mpfrPrecision);
	mpfr_set(bail_T, temp, GMP_RNDN);

	mpfr_clear(temp);
}

void MandelbrotFractal(double& a, double& b, double ca, double cb){
	double aa = a * a - b * b;
	double bb = 2 * a * b;
	a = aa + ca;
	b = bb + cb;
}

void BurningShipFractal(double& a, double& b, double ca, double cb){
	double aa = a * a - b * b;
	double bb = 2 * abs(a * b);
	a = aa + ca;
	b = bb - cb;
}
void CelticFractal(double& a, double& b, double ca, double cb){
	double aa = abs(a * a - b * b);
	double bb = 2 * a * b;
	a = aa + ca;
	b = bb + cb;
}

void CalculateFractal(uint start, uint end)
{
	// Store colortimer value to animate color methods
	float colortime = 0.f;
	if(animated)
		colortime = colortimer.getElapsedTime().asSeconds();

	for (uint y = 0; y < WIN_HEIGHT; y++)
	{
		for (uint x = start; x < end; x++)
		{
			double a = (x + offsetX) / zmx1 - zmx2; // X with Pan and Zoom;
			double b = zmy2 - (y + offsetY) / zmy1; // Y with Pan and Zoom;
			double ca = a;							// Store Constant a and b
			double cb = b;
			double n = 0;							// To store Iterations
			double absOld = 0.0;
			double convergeNumber = maxiterations; 	// Changes if the while loop breaks due to non-convergence
			std::complex<double> der(1.0,1.0); 		// To store derivative
			std::complex<double> z(ca,cb);			// To store complex z
			bool inside=false;

			while (n < maxiterations)
			{
				double abs = sqrt(a * a + b * b);

				// z2 + c
				switch(fractalType){
					case MANDELBROT:
						MandelbrotFractal(a, b, ca, cb);
						break;
					case BURNINGSHIP:
						BurningShipFractal(a, b, ca, cb);
						break;
					case CELTIC:
						CelticFractal(a, b, ca, cb);
						break;
					default:
						MandelbrotFractal(a, b, ca, cb);
						break;
				}

				// Interior Detection
				der = der*2.0*z;
				z = std::complex<double>(a,b);
				if(sqrt(der.real()*der.real()+der.imag()*der.imag()) < eps*eps){
					n = maxiterations;
					inside=true;
					break;
				}

				// Outside of set
				if (abs > escapeRadius)
				{
					// Measure how much we exceeded the maximum
					double diffToLast = abs - absOld;
					double diffToMax = 4.0 - absOld;
					convergeNumber = n + diffToMax / diffToLast;
					break;
				}
				n++;
				absOld = abs;
			}

			// We color each pixel based on how long it takes to get to infinity
			// If we never got there, pick the interiorColor
			if (n == maxiterations)
			{
				if(inside)
					vertexarrayPoints[x + y * WIN_WIDTH].color = sf::Color(interiorColor.r*0.5,interiorColor.g*0.5,interiorColor.b*0.5,255);
				else
				vertexarrayPoints[x + y * WIN_WIDTH].color = interiorColor;
			}
			else
			{
				// Map convergence to smooth brightness
				double brightness = ReMap(convergeNumber, 0, maxiterations, 0, 1);
				brightness = ReMap(sqrt(brightness), 0, 1, 0, 255);
				//double smooth = (n + 2 - log(log(absOld))/log(2.0));
				//double smooth = n+2 - log(log(z.real()*z.real()+z.imag()*z.imag()))/log(2.0);

				switch (colorMethod)
				{
					case 0: // Single Color
						vertexarrayPoints[x + y * WIN_WIDTH].color = sf::Color(brightness * rAmount, brightness * gAmount, brightness * bAmount, 255);
						break;
					case 1: // Palette
						vertexarrayPoints[x + y * WIN_WIDTH].color = palette[(int)(brightness + colortime) % 16];
						break;
					case 2: // Smooth Color with sine
						vertexarrayPoints[x + y * WIN_WIDTH].color = sf::Color((sf::Uint8)(sin(0.3 * brightness + 0 + colortime) * 127 + 127),
																				(sf::Uint8)(sin(0.3 * brightness + 2 + colortime) * 127 + 127),
																				(sf::Uint8)(sin(0.3 * brightness + 4 + colortime) * 127 + 127),
																				255);
						break;
					case 3: // HSV
						HsvColor hsv;
						RgbColor rgb;
						hsv.h = int(brightness + colortime) % 255;
						hsv.s = 255;
						hsv.v = 255;
						rgb = HsvToRgb(hsv);
						vertexarrayPoints[x + y * WIN_WIDTH].color = sf::Color(rgb.r, rgb.g, rgb.b, 255);
						break;
					case 4: // Palette 2 Fire
						vertexarrayPoints[x + y * WIN_WIDTH].color = palette2[(int)(brightness + colortime) % 69];
						break;
					case 5: // From Image
						vertexarrayPoints[x + y * WIN_WIDTH].color = image.getPixel((unsigned int)(z.real()*10)%image.getSize().x, (unsigned int)(z.imag()*10)%image.getSize().y);
						break;
					default: // Default to Single color method
						vertexarrayPoints[x + y * WIN_WIDTH].color = sf::Color(brightness * rAmount, brightness * gAmount, brightness * bAmount, 255);
						break;
				}

				// Normal Map
				if(normalMap){
					u = z/der;
					u = u/abs(u);
					t = u.real()*v.real() + u.imag()*v.imag() + lightHeight;
					t = t/(1+lightHeight);
					if(t<0) t=0;
					if(colorMethod==0)	// Single Color NormalMap
						vertexarrayPoints[x + y * WIN_WIDTH].color = LerpColor(sf::Color::Black,sf::Color(rAmount*255,gAmount*255,bAmount*255,255),t);
					else
						vertexarrayPoints[x + y * WIN_WIDTH].color = LerpColor(sf::Color::Black,vertexarrayPoints[x + y * WIN_WIDTH].color,t);
				}
			}
		}
	}
}
void CalculateFractalMPFR(uint start, uint end)
{
	mpfr_t a2_M, b2_M, a_M, b_M, aa_M, bb_M, ca_M, cb_M, y_M, x_M, abs_M, absold_M, difftolast_M, difftomax_M;

	mpfr_init2(a2_M, mpfrPrecision);
	mpfr_init2(b2_M, mpfrPrecision);
	mpfr_init2(a_M, mpfrPrecision);
	mpfr_init2(b_M, mpfrPrecision);
	mpfr_init2(aa_M, mpfrPrecision);
	mpfr_init2(bb_M, mpfrPrecision);
	mpfr_init2(ca_M, mpfrPrecision);
	mpfr_init2(cb_M, mpfrPrecision);
	mpfr_init2(x_M, mpfrPrecision);
	mpfr_init2(y_M, mpfrPrecision);
	mpfr_init2(abs_M, mpfrPrecision);
	mpfr_init2(absold_M, mpfrPrecision);
	mpfr_init2(difftolast_M, mpfrPrecision);
	mpfr_init2(difftomax_M, mpfrPrecision);

	// Store colortimer value to animate color methods
	float colortime = 0.f;
	if(animated)
		colortime = colortimer.getElapsedTime().asSeconds();

	for (uint y = 0; y < WIN_HEIGHT; y++)
	{
		mpfr_set_flt(y_M, y, GMP_RNDN);
		for (uint x = start; x < end; x++)
		{
			mpfr_set_flt(x_M, x, GMP_RNDN);

			//double a = (x + offsetX) / zmx1 - zmx2; // X with Pan and Zoom;
			mpfr_add(a_M, x_M, offsetX_T, GMP_RNDN);
			mpfr_div(a_M, a_M,zmx1_T, GMP_RNDN);
			mpfr_sub(a_M, a_M, zmx2_T, GMP_RNDN);

			//double b = zmy2 - (y + offsetY) / zmy1; // Y with Pan and Zoom;
			mpfr_add(b_M, y_M, offsetY_T, GMP_RNDN);
			mpfr_div(b_M, b_M, zmy1_T, GMP_RNDN);
			mpfr_sub(b_M, zmy2_T, b_M, GMP_RNDN);

			//double ca = a;							// Store Constant a and b
			mpfr_set(ca_M, a_M, GMP_RNDN);

			//double cb = b;
			mpfr_set(cb_M, b_M, GMP_RNDN);

			double n = 0;							// To store Iterations
			//double absOld = 0.0;
			mpfr_set_d(absold_M, 0.0, GMP_RNDN);
			double convergeNumber = maxiterations; 	// Changes if the while loop breaks due to non-convergence
			std::complex<double> der(1.0,1.0); 		// To store derivative
			std::complex<double> z(mpfr_get_d(ca_M, GMP_RNDN),mpfr_get_d(cb_M,GMP_RNDN));			// To store complex z
			bool inside=false;

			while (n < maxiterations)
			{
				//double abs = sqrt(a * a + b * b);
				mpfr_mul(a2_M, a_M, a_M, GMP_RNDN);
				mpfr_mul(b2_M, b_M, b_M, GMP_RNDN);
				mpfr_add(abs_M, a2_M, b2_M, GMP_RNDN);
				mpfr_sqrt(abs_M, abs_M, GMP_RNDN);

				// z2 + c
				switch(fractalType){
				case MANDELBROT:
					//double aa = a * a - b * b;
					mpfr_sub(aa_M, a2_M, b2_M, GMP_RNDN);

					//double bb = 2 * a * b;
					mpfr_mul_d(bb_M, a_M, 2.0, GMP_RNDN);
					mpfr_mul(bb_M, bb_M, b_M, GMP_RNDN);

					//a = aa + ca;
					mpfr_add(a_M, aa_M, ca_M, GMP_RNDN);
					//b = bb + cb;
					mpfr_add(b_M, bb_M, cb_M, GMP_RNDN);
					break;
				case BURNINGSHIP:
					//double aa = a * a - b * b;
					mpfr_sub(aa_M, a2_M, b2_M, GMP_RNDN);

					//double bb = 2 * abs(a * b);
					mpfr_mul(bb_M, a_M, b_M, GMP_RNDN);
					mpfr_abs(bb_M, bb_M, GMP_RNDN);
					mpfr_mul_d(bb_M, bb_M, 2.0, GMP_RNDN);

					//a = aa + ca;
					mpfr_add(a_M, aa_M, ca_M, GMP_RNDN);
					//b = bb - cb;
					mpfr_sub(b_M, bb_M, cb_M, GMP_RNDN);
					break;
				case CELTIC:
					//double aa = abs(a * a - b * b);
					mpfr_sub(aa_M, a2_M, b2_M, GMP_RNDN);
					mpfr_abs(aa_M, aa_M, GMP_RNDN);

					//double bb = 2 * a * b;
					mpfr_mul_d(bb_M, a_M, 2.0, GMP_RNDN);
					mpfr_mul(bb_M, bb_M, b_M, GMP_RNDN);

					//a = aa + ca;
					mpfr_add(a_M, aa_M, ca_M, GMP_RNDN);
					//b = bb + cb;
					mpfr_add(b_M, bb_M, cb_M, GMP_RNDN);
					break;
				default:
					//double aa = a * a - b * b;
					mpfr_sub(aa_M, a2_M, b2_M, GMP_RNDN);

					//double bb = 2 * a * b;
					mpfr_mul_d(bb_M, a_M, 2.0, GMP_RNDN);
					mpfr_mul(bb_M, bb_M, b_M, GMP_RNDN);

					//a = aa + ca;
					mpfr_add(a_M, aa_M, ca_M, GMP_RNDN);
					//b = bb + cb;
					mpfr_add(b_M, bb_M, cb_M, GMP_RNDN);
					break;
				}

				// Interior Detection
				der = der*2.0*z;
				z = std::complex<double>(mpfr_get_d(a_M,GMP_RNDN),mpfr_get_d(b_M, GMP_RNDN));

				if(sqrt(der.real()*der.real()+der.imag()*der.imag()) < eps*eps){
					n = maxiterations;
					inside=true;
					break;
				}

				// Outside of set
				if (mpfr_greater_p(abs_M,bail_T))
				{
					// Measure how much we exceeded the maximum
					//double diffToLast = abs - absOld;
					mpfr_sub(difftolast_M, abs_M,absold_M, GMP_RNDN);

					//double diffToMax = 4.0 - absOld;
					mpfr_d_sub(difftomax_M, 4.0, absold_M, GMP_RNDN);
					convergeNumber = n + mpfr_get_d(difftomax_M,GMP_RNDN) / mpfr_get_d(difftolast_M,GMP_RNDN);
					break;
				}
				n++;
				mpfr_set(absold_M,abs_M,GMP_RNDN);
			}

			// We color each pixel based on how long it takes to get to infinity
			// If we never got there, pick the interiorColor
			if (n == maxiterations)
			{
				if(inside)
					vertexarrayPoints[x + y * WIN_WIDTH].color = sf::Color(interiorColor.r*0.5,interiorColor.g*0.5,interiorColor.b*0.5,255);
				else
				vertexarrayPoints[x + y * WIN_WIDTH].color = interiorColor;
			}
			else
			{
				// Map convergence to smooth brightness
				double brightness = ReMap(convergeNumber, 0, maxiterations, 0, 1);
				brightness = ReMap(sqrt(brightness), 0, 1, 0, 255);
				//double smooth = (n + 2 - log(log(absOld))/log(2.0));
				//double smooth = n+2 - log(log(z.real()*z.real()+z.imag()*z.imag()))/log(2.0);

				switch (colorMethod)
				{
					case 0: // Single Color
						vertexarrayPoints[x + y * WIN_WIDTH].color = sf::Color(brightness * rAmount, brightness * gAmount, brightness * bAmount, 255);
						break;
					case 1: // Palette
						vertexarrayPoints[x + y * WIN_WIDTH].color = palette[(int)(brightness + colortime) % 16];
						break;
					case 2: // Smooth Color with sine
						vertexarrayPoints[x + y * WIN_WIDTH].color = sf::Color((sf::Uint8)(sin(0.3 * brightness + 0 + colortime) * 127 + 127),
																				(sf::Uint8)(sin(0.3 * brightness + 2 + colortime) * 127 + 127),
																				(sf::Uint8)(sin(0.3 * brightness + 4 + colortime) * 127 + 127),
																				255);
						break;
					case 3: // HSV
						HsvColor hsv;
						RgbColor rgb;
						hsv.h = int(brightness + colortime) % 255;
						hsv.s = 255;
						hsv.v = 255;
						rgb = HsvToRgb(hsv);
						vertexarrayPoints[x + y * WIN_WIDTH].color = sf::Color(rgb.r, rgb.g, rgb.b, 255);
						break;
					case 4: // Palette 2 Fire
						vertexarrayPoints[x + y * WIN_WIDTH].color = palette2[(int)(brightness + colortime) % 69];
						break;
					//case 5: // From Image
					//	vertexarrayPoints[x + y * WIN_WIDTH].color = image.getPixel((unsigned int)(z.real()*10)%image.getSize().x, (unsigned int)(z.imag()*10)%image.getSize().y);
					//	break;
					default: // Default to Single color method
						vertexarrayPoints[x + y * WIN_WIDTH].color = sf::Color(brightness * rAmount, brightness * gAmount, brightness * bAmount, 255);
						break;
				}
				// Normal Map
				if(normalMap){
					u = z/der;
					u = u/abs(u);
					t = u.real()*v.real() + u.imag()*v.imag() + lightHeight;
					t = t/(1+lightHeight);
					if(t<0) t=0;
					if(colorMethod==0)	// Single Color NormalMap
						vertexarrayPoints[x + y * WIN_WIDTH].color = LerpColor(sf::Color::Black,sf::Color(rAmount*255,gAmount*255,bAmount*255,255),t);
					else
						vertexarrayPoints[x + y * WIN_WIDTH].color = LerpColor(sf::Color::Black,vertexarrayPoints[x + y * WIN_WIDTH].color,t);
				}
			}
		}
	}
	mpfr_clear(a2_M);
	mpfr_clear(b2_M);
	mpfr_clear(a_M);
	mpfr_clear(b_M);
	mpfr_clear(aa_M);
	mpfr_clear(bb_M);
	mpfr_clear(ca_M);
	mpfr_clear(cb_M);
	mpfr_clear(x_M);
	mpfr_clear(y_M);
	mpfr_clear(abs_M);
}

void CreateFractalThreads()
{
	std::thread t[maxThreads];
	int sectionWidth = WIN_WIDTH / maxThreads;
	int start, end;

	for (size_t i = 0; i < maxThreads; i++)
	{
		start = i * sectionWidth;
		end = (i + 1) * sectionWidth;
		if(useMPFR)
			t[i] = std::thread(CalculateFractalMPFR, start, end);
		else
			t[i] = std::thread(CalculateFractal, start, end);
	}

	for (size_t i = 0; i < maxThreads; i++)
		t[i].join();

	canCalculateFractal = false;
}

void ResetView()
{
	offsetX = 0.0; // Used for Panning
	offsetY = 0.0;
	zmx1 = WIN_WIDTH / 4; // Used for Zooming
	zmx2 = 2;
	zmy1 = WIN_HEIGHT / 4;
	zmy2 = 2;
	zmAmount = 1.1;
	zoomIn = false;
	zoomOut = false;
	colortimer.restart();
	maxiterations=256;
	currentIterations.setString(to_string(maxiterations));

	mpfr_set_d(offsetX_T, offsetX, GMP_RNDN);
	mpfr_set_d(offsetY_T, offsetY, GMP_RNDN);
	mpfr_set_d(zmx1_T, zmx1, GMP_RNDN);
	mpfr_set_d(zmx2_T, zmx2, GMP_RNDN);
	mpfr_set_d(zmy1_T, zmy1, GMP_RNDN);
	mpfr_set_d(zmy2_T, zmy2, GMP_RNDN);
	mpfr_set_d(zmAmount_T, zmAmount, GMP_RNDN);
	mpfr_set_d(t1_T, 0.0, GMP_RNDN);

	canCalculateFractal = true;
}

void ZoomIn(sf::Window& window)
{
	if(!autoZoomIn){
		mousePos = sf::Mouse::getPosition(window); // Get Mouse pos
		double mouseX_D = ((double)mousePos.x - (WIN_WIDTH / 2.0)) * 0.1;
		double mouseY_D = ((double)mousePos.y - (WIN_WIDTH / 2.0)) * 0.1;
		offsetX = offsetX + mouseX_D;
		mpfr_add_d(offsetX_T, offsetX_T, mouseX_D, GMP_RNDN);
		offsetY = offsetY + mouseY_D;
		mpfr_add_d(offsetY_T, offsetY_T, mouseY_D, GMP_RNDN);
	}
	zmx1 = zmx1 * zmAmount;
	mpfr_mul(zmx1_T, zmx1_T, zmAmount_T, GMP_RNDN);

	zmx2 = zmx2 * (1.0 / zmAmount);
	mpfr_d_div(t1_T, 1.0, zmAmount_T, GMP_RNDN);
	mpfr_mul(zmx2_T, zmx2_T, t1_T, GMP_RNDN);

	zmy1 = zmy1 * zmAmount;
	mpfr_mul(zmy1_T, zmy1_T, zmAmount_T, GMP_RNDN);

	zmy2 = zmy2 * (1.0 / zmAmount);
	mpfr_mul(zmy2_T, zmy2_T, t1_T, GMP_RNDN);

	offsetX = offsetX * zmAmount;
	mpfr_mul(offsetX_T, offsetX_T, zmAmount_T, GMP_RNDN);

	offsetY = offsetY * zmAmount;
	mpfr_mul(offsetY_T, offsetY_T, zmAmount_T, GMP_RNDN);
	if (makeVideoFrames)
		TakeScreenshot(window);
	canCalculateFractal = true;
}

void ZoomOut(sf::Window& window)
{
	mousePos = sf::Mouse::getPosition(window); // Get Mouse pos
	double mouseX_D = ((double)mousePos.x - (WIN_WIDTH / 2.0)) * 0.1;
	double mouseY_D = ((double)mousePos.y - (WIN_WIDTH / 2.0)) * 0.1;
	offsetX = offsetX + mouseX_D;
	mpfr_add_d(offsetX_T, offsetX_T, mouseX_D, GMP_RNDN);
	offsetY = offsetY + mouseY_D;
	mpfr_add_d(offsetY_T, offsetY_T, mouseY_D, GMP_RNDN);

	zmx1 = zmx1 / zmAmount;
	mpfr_div(zmx1_T, zmx1_T, zmAmount_T, GMP_RNDN);

	zmx2 = zmx2 / (1.0 / zmAmount);
	mpfr_d_div(t1_T, 1.0, zmAmount_T, GMP_RNDN);
	mpfr_div(zmx2_T, zmx2_T, t1_T, GMP_RNDN);

	zmy1 = zmy1 / zmAmount;
	mpfr_div(zmy1_T, zmy1_T, zmAmount_T, GMP_RNDN);

	zmy2 = zmy2 / (1.0 / zmAmount);
	mpfr_div(zmy2_T, zmy2_T, t1_T, GMP_RNDN);

	offsetX = offsetX / zmAmount;
	mpfr_div(offsetX_T, offsetX_T, zmAmount_T, GMP_RNDN);
	offsetY = offsetY / zmAmount;
	mpfr_div(offsetY_T, offsetY_T, zmAmount_T, GMP_RNDN);

	canCalculateFractal = true;
}