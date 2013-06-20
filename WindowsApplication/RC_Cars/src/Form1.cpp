#include "StdAfx.h"
#include "Form1.h"

RC_Cars::Form1::Form1(void){
	InitializeComponent();
	KPs = 0.8;
	Tns = 9999;
	Tvs = 0;
	KPt = 0.01;
	Tnt = 30;
	Tvt = 0;
	time1 = 0;
	uSteering = 0;
	wRadius = 500;
}
RC_Cars::Form1::~Form1()
{
	if (components)
	{
		delete components;
	}
}
System::Void RC_Cars::Form1::button_start_Click(System::Object^  sender, System::EventArgs^  e) {
	this->button_start->Enabled = false;
	this->button_stop->Enabled = true;
	this->button_stop->Focus();
	this->timer1->Enabled = true;
	this->chart1->Series["radius"]->Points->Clear();
	this->chart1->Series["setpoint"]->Points->Clear();
	this->chart1->Series["output"]->Points->Clear();
	backgroundWorker1->RunWorkerAsync();
}
System::Void RC_Cars::Form1::button_stop_Click(System::Object^  sender, System::EventArgs^  e) {
	//initializes backgroundWorker1 cancellation
	this->trackBar1->Value = 0;
	this->backgroundWorker1->CancelAsync();
	this->timer1->Enabled = false;
	this->button_stop->Enabled = false;
	this->button_start->Enabled = true;
	this->button_start->Focus();
}
System::Void RC_Cars::Form1::backgroundWorker1_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) {
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
	std::ofstream DataTxt;		// txt-file for writing X, Y, D

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
	double X, Y, D;
	ViconClient.getCoordinates("Car1", &X, &Y, &D);
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
		ViconClient.getCoordinates("Car1", &X, &Y, &D);
		this->X = X;
		this->Y = Y;
		this->D = D;

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
	}
	mycomport->WriteData( 30, 1);		//Stop motor
	mycomport->WriteData(128 +15, 1);	//steering angle = 0
	delete mycomport;
	DataTxt.std::ofstream::close();
}
System::Void RC_Cars::Form1::pictureBox1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
	int x, y, xCenter, yCenter;
	Pen^ pen1=gcnew Pen(Color::Black, 2);
	Pen^ pen2=gcnew Pen(Color::Red, 3);
	//draw red cross
	xCenter = (this->pictureBox1->Size.Width / 2);
	yCenter = (this->pictureBox1->Size.Height / 2);
	x = (int) (X/5) + xCenter;
	y = (int)-(Y/5) + yCenter;
	e->Graphics->DrawEllipse(pen1, xCenter-(int)(wRadius/5), yCenter-(int)(wRadius/5), (int)wRadius/2.5, (int)wRadius/2.5);
	e->Graphics->DrawLine(pen2, x-6, y  , x+7, y  );
	e->Graphics->DrawLine(pen2, x  , y-6, x  , y+7);
	delete pen2;
}
System::Void RC_Cars::Form1::button1_Click(System::Object^  sender, System::EventArgs^  e) {
	//sets treckBar1 and throttle to 30 => motor stop
	this->trackBar1->Value = 0;
	wVelocity = 0;
}
System::Void RC_Cars::Form1::textBox1_Leave(System::Object^  sender, System::EventArgs^  e) {
	if (!(Double::TryParse(textBox1->Text, KPs))){
		MessageBox::Show("Invalid value for KP-Steering");
		this->textBox1->Text = "0";
	}
}
System::Void RC_Cars::Form1::textBox2_Leave(System::Object^  sender, System::EventArgs^  e) {
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
System::Void RC_Cars::Form1::textBox3_Leave(System::Object^  sender, System::EventArgs^  e) {
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
System::Void RC_Cars::Form1::textBox4_Leave(System::Object^  sender, System::EventArgs^  e) {
	if (!(Double::TryParse(textBox4->Text, KPt))){
		MessageBox::Show("Invalid value for KP-Throttle");
		this->textBox4->Text = "0";
	}
}
System::Void RC_Cars::Form1::textBox5_Leave(System::Object^  sender, System::EventArgs^  e) {
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
System::Void RC_Cars::Form1::textBox6_Leave(System::Object^  sender, System::EventArgs^  e) {
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
System::Void RC_Cars::Form1::textBox7_Leave(System::Object^  sender, System::EventArgs^  e) {
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
System::Void RC_Cars::Form1::trackBar1_Scroll(System::Object^  sender, System::EventArgs^  e) {
	//throttle = 0 => max forward; throttle = 60 max backward; trackBar1 = [0;60]
	wVelocity = 10 * this->trackBar1->Value;
}
System::Void RC_Cars::Form1::timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
	//graph
	this->chart1->Series["radius"]->Points->AddXY(time1,radius);
	this->chart1->Series["setpoint"]->Points->AddXY(time1,wRadius);
	this->chart1->Series["output"]->Points->AddXY(time1,uSteering);

	//redraw pictureBox1
	pictureBox1->Invalidate();
}