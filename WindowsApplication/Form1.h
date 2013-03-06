#pragma once
#include "ClientCreator.h"
#include <SerialClass.h>
#include <PID_Controller.h>

namespace RC_Cars {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Zusammenfassung für Form1
	/// </summary>

	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			
			createClient();
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				disconnectClient();
				delete components;
			}
		}
	private: System::ComponentModel::BackgroundWorker^  backgroundWorker1;
	protected: 
	private: System::Windows::Forms::Button^  button_start;
	private: System::Windows::Forms::PictureBox^  pictureBox1;
	private: System::Windows::Forms::Button^  button_stop;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  textBox2;
	private: System::Windows::Forms::TextBox^  textBox3;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::TrackBar^  trackBar1;
	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Panel^  panel2;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Panel^  panel3;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::TextBox^  textBox4;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::TextBox^  textBox5;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::TextBox^  textBox6;

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
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
			this->label4 = (gcnew System::Windows::Forms::Label());
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
			this->pictureBox1->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox1.BackgroundImage")));
			this->pictureBox1->Location = System::Drawing::Point(174, 12);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(800, 800);
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
			this->textBox1->Location = System::Drawing::Point(40, 35);
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
			this->label1->Location = System::Drawing::Point(7, 38);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(30, 13);
			this->label1->TabIndex = 4;
			this->label1->Text = L"KP =";
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(40, 62);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(28, 20);
			this->textBox2->TabIndex = 6;
			this->textBox2->Text = L"9999";
			this->textBox2->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->textBox2->Leave += gcnew System::EventHandler(this, &Form1::textBox2_Leave);
			// 
			// textBox3
			// 
			this->textBox3->Location = System::Drawing::Point(40, 89);
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
			this->label2->Location = System::Drawing::Point(8, 65);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(29, 13);
			this->label2->TabIndex = 7;
			this->label2->Text = L"Tn =";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(8, 92);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(29, 13);
			this->label3->TabIndex = 8;
			this->label3->Text = L"Tv =";
			// 
			// trackBar1
			// 
			this->trackBar1->LargeChange = 1;
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
			this->panel1->Controls->Add(this->label4);
			this->panel1->Controls->Add(this->textBox1);
			this->panel1->Controls->Add(this->label1);
			this->panel1->Controls->Add(this->label3);
			this->panel1->Controls->Add(this->textBox2);
			this->panel1->Controls->Add(this->label2);
			this->panel1->Controls->Add(this->textBox3);
			this->panel1->Location = System::Drawing::Point(92, 133);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(76, 120);
			this->panel1->TabIndex = 3;
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
			// panel2
			// 
			this->panel2->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->panel2->Controls->Add(this->button1);
			this->panel2->Controls->Add(this->label5);
			this->panel2->Controls->Add(this->trackBar1);
			this->panel2->Location = System::Drawing::Point(11, 42);
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
			this->panel3->Location = System::Drawing::Point(12, 133);
			this->panel3->Name = L"panel3";
			this->panel3->Size = System::Drawing::Size(74, 120);
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
			this->textBox4->Location = System::Drawing::Point(40, 35);
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
			this->label7->Location = System::Drawing::Point(7, 38);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(30, 13);
			this->label7->TabIndex = 4;
			this->label7->Text = L"KP =";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(8, 92);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(29, 13);
			this->label8->TabIndex = 8;
			this->label8->Text = L"Tv =";
			// 
			// textBox5
			// 
			this->textBox5->Location = System::Drawing::Point(40, 62);
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
			this->label9->Location = System::Drawing::Point(8, 65);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(29, 13);
			this->label9->TabIndex = 7;
			this->label9->Text = L"Tn =";
			// 
			// textBox6
			// 
			this->textBox6->Location = System::Drawing::Point(40, 89);
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
			this->ClientSize = System::Drawing::Size(990, 830);
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
private: System::Void button_start_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->button_start->Enabled = false;
				 this->button_stop->Enabled = true;
				 this->button_stop->Focus();
				 backgroundWorker1->RunWorkerAsync();
			 }
private: System::Void pictureBox1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
			 
			 Pen^ pen=gcnew Pen(Color::Red, 3);
			 //draw red cross
			 for (int i=0; i<5;i++){
				e->Graphics->DrawLine(pen,(int)X[i]/5+400-6,(int)-Y[i]/5+400,(int)X[i]/5+400+7,(int)-Y[i]/5+400);
				e->Graphics->DrawLine(pen,(int)X[i]/5+400,(int)-Y[i]/5+400-6,(int)X[i]/5+400,(int)-Y[i]/5+400+7);
			 }
			 delete pen;
		 }
private: System::Void backgroundWorker1_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) {
			const int wRadius = 500;	//set-point radius;
			int steering=15, oldSteering= 128 + 15, throttle=30, oldThrottle=30;
			double uSteering, uThrottle, radius, velocity;
			double Xold, Yold;				// previous step coordinates
			double dt;						// elapsed time since last frame [sec.]
			LARGE_INTEGER freq;				// ticks per second, for accurate time measurement
			LARGE_INTEGER t1, t2, t3;       // ticks, for accurate time measurement
			double vD, dD;					// direction of velocity vector, angel difference

			// Create txt-File
			DataTxt.std::ofstream::open("Data.txt"); 
			DataTxt << "time\tX1\tY1\tRZ1\tX2\tY2\tRZ2\tX3\tY3\tRZ3\tX4\tY4\tRZ4\tX5\tY5\tRZ5\n" ;
			
			QueryPerformanceFrequency(&freq); // get ticks per second
			QueryPerformanceCounter(&t1);	// get starting time
			QueryPerformanceCounter(&t3);

			Serial *mycomport = new Serial("COM3");		//use the Arduino COM Port here, can be found in Arduino IDE
			PID_Controller controller1(steeringMin, steeringMax);
			PID_Controller controller2(throttleMin, throttleMax);

			//set old coordinates to current coordinates
			getCoordinates();
			Xold = X[0];
			Yold = Y[0];

			while (true){
				//check for cancellation
				if (this->backgroundWorker1->CancellationPending)
					break;

				//get current time
				QueryPerformanceCounter(&t2);	

				// elapsed time since last frame [sec.]
				dt = ( (double)(t2.QuadPart - t3.QuadPart) ) / freq.QuadPart; 

				//get coordinates (waits for new frame (Vicon Tracker frame rate) )
				getCoordinates();
				
				radius = sqrt(X[0]*X[0]+Y[0]*Y[0]);										// [mm]
				velocity = sqrt((X[0]-Xold)*(X[0]-Xold)+(Y[0]-Yold)*(Y[0]-Yold)) / dt;	// [mm/sec]

				//detect direction of velocity vector
				vD = atan2(Y[0]-Yold,X[0]-Xold);	//vD = [-pi;pi]
				dD = abs(D[0] - vD);				//angle difference
				if ((dD > (pi/2)) && (dD < (3/2*pi)))	// car moves backward
					velocity = -velocity; 

				//PID-controller
				uSteering = controller1.step(wRadius, radius, KPs, Tns, Tvs, dt); 
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
				DataTxt << ((double)(t2.QuadPart - t1.QuadPart)) / freq.QuadPart << "\t" //time
						<< X[0] << "\t"	<< Y[0] << "\t"	<< D[0] << "\t"	
						<< X[1] << "\t"	<< Y[1] << "\t"	<< D[1] << "\t"
						<< X[2] << "\t"	<< Y[2] << "\t"	<< D[2] << "\t"
						<< X[3] << "\t"	<< Y[3] << "\t"	<< D[3] << "\t"
						<< X[4] << "\t"	<< Y[4] << "\t"	<< D[4] << "\n";

				//prepare next loop
				t3 = t2;
				Xold = X[0];
				Yold = Y[0];
				
				//redraw pictureBox1
				pictureBox1->Invalidate();
			}
			mycomport->WriteData( 30, 1);		//Stop motor
			mycomport->WriteData(128 +15, 1);	//steering angle = 0
			delete mycomport;
		 }
private: System::Void button_stop_Click(System::Object^  sender, System::EventArgs^  e) {
			 //initializes backgroundWorker1 cancellation
			 this->trackBar1->Value = 0;
			 this->backgroundWorker1->CancelAsync();
			 this->button_stop->Enabled = false;
			 this->button_start->Enabled = true;
			 this->button_start->Focus();
		 }
private: System::Void backgroundWorker1_RunWorkerCompleted(System::Object^  sender, System::ComponentModel::RunWorkerCompletedEventArgs^  e) {
			 DataTxt.std::ofstream::close();
			 //MessageBox::Show("");
		 }
private: System::Void textBox1_Leave(System::Object^  sender, System::EventArgs^  e) {
			 KPs = System::Convert::ToDouble(this->textBox1->Text);
		 }
private: System::Void textBox2_Leave(System::Object^  sender, System::EventArgs^  e) {
			 double x = System::Convert::ToDouble(this->textBox2->Text);
			 //Tn must be greater than zero
			 if (x>0)
				 Tns = x;
			 else{
				 this->textBox2->Text = "1";
				 Tns = 1;
			 }
		 }
private: System::Void textBox3_Leave(System::Object^  sender, System::EventArgs^  e) {
			 double x = System::Convert::ToDouble(this->textBox3->Text);
			 //Tv must not be less than zero
			 if (x<0){
				 this->textBox3->Text = "0";
				 Tvs = 0;
			 }
			 else
				 Tvs = x;
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
			 KPt = System::Convert::ToDouble(this->textBox4->Text);
		 }
private: System::Void textBox5_Leave(System::Object^  sender, System::EventArgs^  e) {
			 double x = System::Convert::ToDouble(this->textBox5->Text);
			 //Tn must be greater than zero
			 if (x>0)
				 Tnt = x;
			 else{
				 this->textBox5->Text = "1";
				 Tnt = 1;
			 }
		 }
private: System::Void textBox6_Leave(System::Object^  sender, System::EventArgs^  e) {
			 double x = System::Convert::ToDouble(this->textBox6->Text);
			 //Tv must not be less than zero
			 if (x<0){
				 this->textBox6->Text = "0";
				 Tvt = 0;
			 }
			 else
				 Tvt = x;
		 }
};
}

