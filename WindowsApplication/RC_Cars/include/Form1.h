/** \mainpage 
 * \section intro Introduction
 *
 * This is the introduction.
 *
 * \subsection step1 Step 1:
 *  
 * etc...
 */

/** \file Form1.h
	\brief This header file describes a Windows Form

	For further information got to RC_Cars::Form1.
*/
#pragma once
#include "ClientCreator.h"
#include <SerialClass.h>
#include <iostream>
#include "time.h"
#include <PID_Controller.h>
#include <fstream>	// For txt-File
#define pi 3.14159265358979323846

double X;	///<x-coordinate
double Y;	///<y-coordinate
double D;	///<direction
double KPs = 0.8;	///<proportional gain (steering)
double Tns = 9999;	///<integral time (steering)
double Tvs = 0;		///<derivative time (steering)
double KPt = 0.01;	///<proportional gain (throttle)
double Tnt = 30;		///<integral time (throttle)
double Tvt = 0;		///<derivative time (throttle)
double time1 = 0;			// total elapsed time
double radius;				// distance between the origin and the car
double uSteering = 0;			// controller output steering
int wRadius = 500;	///<set point radius
int wVelocity;		///<set point velocity
const double steeringMax = 14;	///<high controller output boundary (steering)
const double throttleMax = 30;	///<high controller output boundary (throttle)
const double steeringMin =-14;	///<low controller output boundary (steering)
const double throttleMin =-30;	///<low controller output boundary (throttle)
std::ofstream DataTxt;			///<txt-file for writing X, Y, D

namespace RC_Cars {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// This Windows Form Application tracks and controls a RC-car
	/**The goal of this form application is to track a RC-car with the help of the Vicon system and to control
	 the cars steering to make it drive circles. A second controller manages the speed. The controller parameters
	 and set points can be changed at any time via text boxes or a track bar. The position of the car is displayed
	 in a picture box. Whilst running the coordinates, orientation and velocity of the car are saved in a text file
	 for every loop.*/

	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		///Constructor
		/**First the constructor initializes all form components (buttons, text boxes, etc.) by calling InitializeComponent().*/
		Form1(void)
		{
			InitializeComponent();
		}

	protected:
		///Destructor
		/**The destructor deletes all components of the form.*/
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	///A BackgroundWorker is an object given in VS to run an additional thread
	/**This thread is needed to continuously run the functions to track and control the car, while the main thread
	manages in-/output of the form.*/
	private: System::ComponentModel::BackgroundWorker^  backgroundWorker1;
	protected: 
	///clicking this button runs backgroundWorker1 (tracking, controlling)
	private: System::Windows::Forms::Button^  button_start;
	///clicking this button cancels backgroundWorker1 (tracking, controlling)
	private: System::Windows::Forms::Button^  button_stop;
	///clicking this button sets the velocity set point to zero (stops the car)
	private: System::Windows::Forms::Button^  button1;
	///This pictureBox is used to visualize the position of the car in the Coordinate system
	private: System::Windows::Forms::PictureBox^  pictureBox1;
	///This textBox displays/sets the value of KP for the steering control
	private: System::Windows::Forms::TextBox^  textBox1;
	///This textBox displays/sets the value of Tn for the steering control
	private: System::Windows::Forms::TextBox^  textBox2;
	///This textBox displays/sets the value of Tv for the steering control
	private: System::Windows::Forms::TextBox^  textBox3;
	///This textBox displays/sets the value of KP for the velocity control
	private: System::Windows::Forms::TextBox^  textBox4;
	///This textBox displays/sets the value of Tn for the velocity control
	private: System::Windows::Forms::TextBox^  textBox5;
	///This textBox displays/sets the value of Tv for the velocity control
	private: System::Windows::Forms::TextBox^  textBox6;
	///This textBox displays/sets the set point radius for the steering control
	private: System::Windows::Forms::TextBox^  textBox7;
	///This trackBar sets the set point of the velocity control
	private: System::Windows::Forms::TrackBar^  trackBar1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::Panel^  panel2;
	private: System::Windows::Forms::Panel^  panel3;

	private: System::ComponentModel::IContainer^  components;
	private:
		


#pragma region Windows Form Designer generated code
		///This funktion intializes all components of the form
		/**The code is automaticly generated and should NOT be changed manually*/
		void InitializeComponent(void)
		{
			this->backgroundWorker1 = (gcnew System::ComponentModel::BackgroundWorker());
			this->button_start = (gcnew System::Windows::Forms::Button());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->button_stop = (gcnew System::Windows::Forms::Button());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->textBox3 = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->trackBar1 = (gcnew System::Windows::Forms::TrackBar());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->textBox7 = (gcnew System::Windows::Forms::TextBox());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->panel3 = (gcnew System::Windows::Forms::Panel());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->textBox4 = (gcnew System::Windows::Forms::TextBox());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->textBox5 = (gcnew System::Windows::Forms::TextBox());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->textBox6 = (gcnew System::Windows::Forms::TextBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar1))->BeginInit();
			this->panel1->SuspendLayout();
			this->panel2->SuspendLayout();
			this->panel3->SuspendLayout();
			this->SuspendLayout();
			// 
			// backgroundWorker1
			// 
			this->backgroundWorker1->WorkerSupportsCancellation = true;
			this->backgroundWorker1->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &Form1::backgroundWorker1_DoWork);
			this->backgroundWorker1->RunWorkerCompleted += gcnew System::ComponentModel::RunWorkerCompletedEventHandler(this, &Form1::backgroundWorker1_RunWorkerCompleted);
			// 
			// button_start
			// 
			this->button_start->Location = System::Drawing::Point(11, 13);
			this->button_start->Name = L"button_start";
			this->button_start->Size = System::Drawing::Size(75, 23);
			this->button_start->TabIndex = 0;
			this->button_start->Text = L"Start";
			this->button_start->UseVisualStyleBackColor = true;
			this->button_start->Click += gcnew System::EventHandler(this, &Form1::button_start_Click);
			// 
			// pictureBox1
			// 
			this->pictureBox1->BackColor = System::Drawing::Color::White;
			this->pictureBox1->Location = System::Drawing::Point(174, 12);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(540, 380);
			this->pictureBox1->TabIndex = 1;
			this->pictureBox1->TabStop = false;
			this->pictureBox1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Form1::pictureBox1_Paint);
			// 
			// button_stop
			// 
			this->button_stop->Enabled = false;
			this->button_stop->Location = System::Drawing::Point(92, 13);
			this->button_stop->Name = L"button_stop";
			this->button_stop->Size = System::Drawing::Size(75, 23);
			this->button_stop->TabIndex = 1;
			this->button_stop->Text = L"Stop";
			this->button_stop->UseVisualStyleBackColor = true;
			this->button_stop->Click += gcnew System::EventHandler(this, &Form1::button_stop_Click);
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(38, 62);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(28, 20);
			this->textBox1->TabIndex = 5;
			this->textBox1->Text = L"0.8";
			this->textBox1->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->textBox1->Leave += gcnew System::EventHandler(this, &Form1::textBox1_Leave);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(5, 65);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(30, 13);
			this->label1->TabIndex = 4;
			this->label1->Text = L"KP =";
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(38, 89);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(28, 20);
			this->textBox2->TabIndex = 6;
			this->textBox2->Text = L"9999";
			this->textBox2->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->textBox2->Leave += gcnew System::EventHandler(this, &Form1::textBox2_Leave);
			// 
			// textBox3
			// 
			this->textBox3->Location = System::Drawing::Point(38, 116);
			this->textBox3->Name = L"textBox3";
			this->textBox3->Size = System::Drawing::Size(28, 20);
			this->textBox3->TabIndex = 7;
			this->textBox3->Text = L"0";
			this->textBox3->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->textBox3->Leave += gcnew System::EventHandler(this, &Form1::textBox3_Leave);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(6, 92);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(29, 13);
			this->label2->TabIndex = 7;
			this->label2->Text = L"Tn =";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(6, 119);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(29, 13);
			this->label3->TabIndex = 8;
			this->label3->Text = L"Tv =";
			// 
			// trackBar1
			// 
			this->trackBar1->LargeChange = 10;
			this->trackBar1->Location = System::Drawing::Point(3, 5);
			this->trackBar1->Maximum = 100;
			this->trackBar1->Name = L"trackBar1";
			this->trackBar1->Size = System::Drawing::Size(149, 45);
			this->trackBar1->TabIndex = 3;
			this->trackBar1->Scroll += gcnew System::EventHandler(this, &Form1::trackBar1_Scroll);
			// 
			// panel1
			// 
			this->panel1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->panel1->Controls->Add(this->label10);
			this->panel1->Controls->Add(this->label4);
			this->panel1->Controls->Add(this->textBox7);
			this->panel1->Controls->Add(this->textBox1);
			this->panel1->Controls->Add(this->label1);
			this->panel1->Controls->Add(this->label3);
			this->panel1->Controls->Add(this->textBox2);
			this->panel1->Controls->Add(this->label2);
			this->panel1->Controls->Add(this->textBox3);
			this->panel1->Location = System::Drawing::Point(92, 163);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(76, 146);
			this->panel1->TabIndex = 3;
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(3, 38);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(35, 13);
			this->label10->TabIndex = 6;
			this->label10->Text = L"radius";
			this->label10->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(11, 8);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(46, 13);
			this->label4->TabIndex = 9;
			this->label4->Text = L"Steering";
			// 
			// textBox7
			// 
			this->textBox7->Location = System::Drawing::Point(38, 35);
			this->textBox7->Name = L"textBox7";
			this->textBox7->Size = System::Drawing::Size(28, 20);
			this->textBox7->TabIndex = 5;
			this->textBox7->Text = L"500";
			this->textBox7->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->textBox7->Leave += gcnew System::EventHandler(this, &Form1::textBox7_Leave);
			// 
			// panel2
			// 
			this->panel2->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->panel2->Controls->Add(this->button1);
			this->panel2->Controls->Add(this->label5);
			this->panel2->Controls->Add(this->trackBar1);
			this->panel2->Location = System::Drawing::Point(11, 72);
			this->panel2->Name = L"panel2";
			this->panel2->Size = System::Drawing::Size(157, 85);
			this->panel2->TabIndex = 2;
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(41, 56);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 4;
			this->button1->Text = L"stop";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(57, 37);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(43, 13);
			this->label5->TabIndex = 12;
			this->label5->Text = L"Throttle";
			// 
			// panel3
			// 
			this->panel3->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->panel3->Controls->Add(this->label6);
			this->panel3->Controls->Add(this->textBox4);
			this->panel3->Controls->Add(this->label7);
			this->panel3->Controls->Add(this->label8);
			this->panel3->Controls->Add(this->textBox5);
			this->panel3->Controls->Add(this->label9);
			this->panel3->Controls->Add(this->textBox6);
			this->panel3->Location = System::Drawing::Point(12, 163);
			this->panel3->Name = L"panel3";
			this->panel3->Size = System::Drawing::Size(74, 146);
			this->panel3->TabIndex = 4;
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(11, 8);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(43, 13);
			this->label6->TabIndex = 9;
			this->label6->Text = L"Throttle";
			// 
			// textBox4
			// 
			this->textBox4->Location = System::Drawing::Point(39, 62);
			this->textBox4->Name = L"textBox4";
			this->textBox4->Size = System::Drawing::Size(30, 20);
			this->textBox4->TabIndex = 5;
			this->textBox4->Text = L"0.01";
			this->textBox4->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->textBox4->Leave += gcnew System::EventHandler(this, &Form1::textBox4_Leave);
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(6, 65);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(30, 13);
			this->label7->TabIndex = 4;
			this->label7->Text = L"KP =";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(7, 119);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(29, 13);
			this->label8->TabIndex = 8;
			this->label8->Text = L"Tv =";
			// 
			// textBox5
			// 
			this->textBox5->Location = System::Drawing::Point(39, 89);
			this->textBox5->Name = L"textBox5";
			this->textBox5->Size = System::Drawing::Size(30, 20);
			this->textBox5->TabIndex = 6;
			this->textBox5->Text = L"30";
			this->textBox5->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->textBox5->Leave += gcnew System::EventHandler(this, &Form1::textBox5_Leave);
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(7, 92);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(29, 13);
			this->label9->TabIndex = 7;
			this->label9->Text = L"Tn =";
			// 
			// textBox6
			// 
			this->textBox6->Location = System::Drawing::Point(39, 116);
			this->textBox6->Name = L"textBox6";
			this->textBox6->Size = System::Drawing::Size(30, 20);
			this->textBox6->TabIndex = 7;
			this->textBox6->Text = L"0";
			this->textBox6->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->textBox6->Leave += gcnew System::EventHandler(this, &Form1::textBox6_Leave);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(722, 612);
			this->Controls->Add(this->panel3);
			this->Controls->Add(this->panel2);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->button_stop);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->button_start);
			this->Name = L"Form1";
			this->Text = L"Form1";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar1))->EndInit();
			this->panel1->ResumeLayout(false);
			this->panel1->PerformLayout();
			this->panel2->ResumeLayout(false);
			this->panel2->PerformLayout();
			this->panel3->ResumeLayout(false);
			this->panel3->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
///This funktion is called when the Start-button is clicked. It calls the backgroundWorker1_DoWork() funktion.
private: System::Void button_start_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->button_start->Enabled = false;
				 this->button_stop->Enabled = true;
				 this->button_stop->Focus();
				 backgroundWorker1->RunWorkerAsync();
			 }
///This funktion is called to redraw pictureBox1
/**A red cross is drawen on pictureBox1 do indicate the position of the car.*/
private: System::Void pictureBox1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
			 int x, y;
			 Pen^ pen=gcnew Pen(Color::Red, 3);
			 //draw red cross
			 x = (int) X/5+300;
			 y = (int)-Y/5+240;
		 	 e->Graphics->DrawLine(pen, x-6, y  , x+7, y  );
			 e->Graphics->DrawLine(pen, x  , y-6, x  , y+7);
			 delete pen;
		 }
///This function runs in the backgroundWorker1 thread.
/**The main work of this application is done by this function.
An infinite loop runs in this function until it receives the cancellation signal. Before entering the loop
two controller objects are created from PID_Controller, a Serial is created and a text file is opened.
In this loop the controlled variables radius (distance between car and origin) and velocity are calculated and 
the elapsed time since the last loop is measured. These values and the controller parameters Kp, Tn and Tv are
necessary to call PID_Controller::step for the steering and the throttle. The controller output values are send to
the Arduino via Serial::WriteData. When the loop is canceld throttle and steering of the car is set to zero and
this function finishes.*/
private: System::Void backgroundWorker1_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) {
			double uThrottle;			// controller output throttle
			int steering=15;			// discrete steering shifted
			int oldSteering= 128 + 15;	// previous loop discrete steering
			int throttle=30;			// discrete throttle shifted
			int oldThrottle=30;			// previous loop discrete throttle
			double velocity;			// velocity of the car
			double Xold;				// X-coordinate of the previous loop
			double Yold;				// Y-coordinate of the previous loop
			double dt;					// elapsed time since last frame [sec.]
			LARGE_INTEGER freq;			// ticks per second, for accurate time measurement
			LARGE_INTEGER t1, t2, t3;	// ticks, for accurate time measurement
			double vD, dD;				// direction of velocity vector, angel difference

			// Create txt-File
			DataTxt.std::ofstream::open("Data.txt"); 
			DataTxt << "time\tX1\tY1\tRZ1\tv\n" ;
			
			QueryPerformanceFrequency(&freq); // get ticks per second
			QueryPerformanceCounter(&t1);	// get starting time
			QueryPerformanceCounter(&t3);

			Serial *mycomport = new Serial("COM3");
			PID_Controller controller1(steeringMin, steeringMax);
			PID_Controller controller2(throttleMin, throttleMax);
			ClientCreator ViconClient;

			//set old coordinates to current coordinates, this prevents an instantaneous jump of the car in the first loop
			ViconClient.getCoordinates("Car1", X, Y, D);
			Xold = X;
			Yold = Y;
			
			while (true){
				//check for cancellation
				if (this->backgroundWorker1->CancellationPending)
					break;

				//get current time
				QueryPerformanceCounter(&t2);
				time1 = ((double)(t2.QuadPart - t1.QuadPart)) / freq.QuadPart;

				// elapsed time since last frame [sec.]
				dt = ( (double)(t2.QuadPart - t3.QuadPart) ) / freq.QuadPart; 

				//get coordinates (waits for new frame (Vicon Tracker frame rate) )
				ViconClient.getCoordinates("Car1", X, Y, D);
				
				radius = sqrt(X*X+Y*Y);										// [mm]
				velocity = sqrt((X-Xold)*(X-Xold)+(Y-Yold)*(Y-Yold)) / dt;	// [mm/sec]

				//detect direction of velocity vector
				vD = atan2(Y-Yold,X-Xold);	//vD = [-pi;pi]
				dD = abs(D - vD);				//angle difference
				if ((dD > (pi/2)) && (dD < (3/2*pi)))	// car moves backward
					velocity = -velocity; 

				//PID-controllers
				uSteering = controller1.step(wRadius , radius, KPs, Tns, Tvs, dt); 
				uThrottle = controller2.step(wVelocity, velocity, KPt, Tnt, Tvt, dt);
							
				//send data to car
				steering = (int) uSteering + 15;
				steering |= 1 << 7;
				if (steering != oldSteering)
					mycomport->WriteData(steering, 1);	//steering
				oldSteering = steering;

				throttle = (int) -uThrottle + 30;
				if (throttle != oldThrottle)
					mycomport->WriteData(throttle, 1);	//throttle
				oldThrottle = throttle;

				//write txt
				DataTxt << time1 << "\t" //time
						<< X << "\t"	<< Y << "\t"	<< D << "\t" 
						<< velocity << "\t" << (int) uThrottle << "\n";

				//prepare next loop
				t3 = t2;
				Xold = X;
				Yold = Y;
				
				//redraw pictureBox1
				pictureBox1->Invalidate();
			}
			mycomport->WriteData( 30, 1);		//Stop motor
			mycomport->WriteData(128 +15, 1);	//steering angle = 0
			delete mycomport;
		 }
///Clicking the Stop button will cancel the backgroundWorker1_DoWork() function.
private: System::Void button_stop_Click(System::Object^  sender, System::EventArgs^  e) {
			 //initializes backgroundWorker1 cancellation
			 this->trackBar1->Value = 0;
			 this->backgroundWorker1->CancelAsync();
			 this->button_stop->Enabled = false;
			 this->button_start->Enabled = true;
			 this->button_start->Focus();
		 }
///This function is called when backgroundWorker1 completed its work and closes the text file.
private: System::Void backgroundWorker1_RunWorkerCompleted(System::Object^  sender, System::ComponentModel::RunWorkerCompletedEventArgs^  e) {
			 DataTxt.std::ofstream::close();
		 }
private: System::Void textBox1_Leave(System::Object^  sender, System::EventArgs^  e) {
			 if (!(Double::TryParse(textBox1->Text, KPs))){
				 MessageBox::Show("Invalid value for KP-Steering");
				 this->textBox1->Text = "0";
			 }
		 }
private: System::Void textBox2_Leave(System::Object^  sender, System::EventArgs^  e) {
			 double x;
			 if (Double::TryParse(textBox2->Text, x)){
				 if (x>0)	//Tn must be greater than zero
					 Tns = x;
				 else{
					 MessageBox::Show("Invalid value for Tn-Steering\n\nTn must be greater than 0");
					 this->textBox2->Text = "9999";
					 Tns = 9999;
				 }
			 }
			 else{
				 MessageBox::Show("Invalid value for Tn-Steering\n\nTn must be greater than 0");
				 this->textBox2->Text = "9999";
				 Tns = 9999;
			 }
		 }
private: System::Void textBox3_Leave(System::Object^  sender, System::EventArgs^  e) {
			 double x;
			 if (Double::TryParse(textBox3->Text, x)){
			 	 if (x<0){	//Tv must not be less than zero
					 MessageBox::Show("Invalid value for Tv-Steering\n\nTv must be equal to 0 or greater");
					 this->textBox3->Text = "0";
					 Tvs = 0;
				 }
				 else
					 Tvs = x;
			 }
			 else{
				 MessageBox::Show("Invalid value for Tv-Steering\n\nTv must be equal to 0 or greater");
				 this->textBox3->Text = "0";
				 Tvs = 0;
			 }
		 }
private: System::Void trackBar1_Scroll(System::Object^  sender, System::EventArgs^  e) {
			 //throttle = 0 => max forward; throttle = 60 max backward; trackBar1 = [0;60]
			 wVelocity = 10 * this->trackBar1->Value;
		 }
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
			 //sets treckBar1 and throttle to 30 => motor stop
			 this->trackBar1->Value = 0;
			 wVelocity = 0;
		 }
private: System::Void textBox4_Leave(System::Object^  sender, System::EventArgs^  e) {
			 if (!(Double::TryParse(textBox4->Text, KPt))){
				 MessageBox::Show("Invalid value for KP-Throttle");
				 this->textBox4->Text = "0";
			 }
		 }
private: System::Void textBox5_Leave(System::Object^  sender, System::EventArgs^  e) {
			 double x;
			 if (Double::TryParse(textBox5->Text, x)){
				 if (x>0)	//Tn must be greater than zero
					 Tnt = x;
				 else{
					 MessageBox::Show("Invalid value for Tn-Throttle\n\nTn must be greater than 0");
					 this->textBox5->Text = "9999";
					 Tnt = 9999;
				 }
			 }
			 else{
				 MessageBox::Show("Invalid value for Tn-Throttle\n\nTn must be greater than 0");
				 this->textBox5->Text = "9999";
				 Tnt = 9999;
			 }
		 }
private: System::Void textBox6_Leave(System::Object^  sender, System::EventArgs^  e) {
			 double x;
			 if (Double::TryParse(textBox6->Text, x)){
			 	 if (x<0){	//Tv must not be less than zero
					 MessageBox::Show("Invalid value for Tv-Throttle\n\nTv must be equal to 0 or greater");
					 this->textBox6->Text = "0";
					 Tvt = 0;
				 }
				 else
					 Tvt = x;
			 }
			 else{
				 MessageBox::Show("Invalid value for Tv-Throttle\n\nTv must be equal to 0 or greater");
				 this->textBox6->Text = "0";
				 Tvt = 0;
			 }
		 }
private: System::Void textBox7_Leave(System::Object^  sender, System::EventArgs^  e) {
			 double x;
			 if (Double::TryParse(textBox7->Text, x)){
				 if (x>0)	//Tn must be greater than zero
					 wRadius = x;
				 else{
					 MessageBox::Show("Invalid value for radius\n\nradius must be greater than 0");
					 this->textBox7->Text = "500";
					 wRadius = 500;
				 }
			 }
			 else{
				 MessageBox::Show("Invalid value for radius\n\nradius must be greater than 0");
				 this->textBox7->Text = "500";
				 wRadius = 500;
			 }
		 }
};
}

