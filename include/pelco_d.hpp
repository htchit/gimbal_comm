class PELCO_D
{
public:
    void Up(unsigned char speed);
    void Down(unsigned char speed);
    void Right(unsigned char speed);
    void Left(unsigned char speed);
    void ZoomIn(unsigned char speed);
    void ZoomOut(unsigned char speed);
    void ZoomStop();
    void Stop();
    void Parse();
    unsigned char tx_buf[7] = {0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};
    unsigned char rx_buf[8];
    unsigned char Zoom_Ratio;
    double Pitch_deg;
    double Yaw_deg;
    bool online;
};

void PELCO_D::Stop()
{
    int calc_sum = 0;
    tx_buf[2] = 0x00;
    tx_buf[3] = 0x00;
    tx_buf[4] = 0x00;
    tx_buf[5] = 0x00;
    calc_sum = tx_buf[1] + tx_buf[2] + tx_buf[3] + tx_buf[4] + tx_buf[5];
    calc_sum %= 0x100;
    tx_buf[6] = calc_sum;
}

void PELCO_D::Up(unsigned char speed)
{
    int calc_sum = 0;
    tx_buf[2] = 0x00;
    tx_buf[3] = 0x08;
    tx_buf[4] = 0x00;
    tx_buf[5] = speed;
    calc_sum = tx_buf[1] + tx_buf[2] + tx_buf[3] + tx_buf[4] + tx_buf[5];
    calc_sum %= 0x100;
    tx_buf[6] = calc_sum;
}

void PELCO_D::Down(unsigned char speed)
{
    int calc_sum = 0;
    tx_buf[2] = 0x00;
    tx_buf[3] = 0x10;
    tx_buf[4] = 0x00;
    tx_buf[5] = speed;
    calc_sum = tx_buf[1] + tx_buf[2] + tx_buf[3] + tx_buf[4] + tx_buf[5];
    calc_sum %= 0x100;
    tx_buf[6] = calc_sum;
}

void PELCO_D::Right(unsigned char speed)
{
    int calc_sum = 0;
    tx_buf[2] = 0x00;
    tx_buf[3] = 0x02;
    tx_buf[4] = speed;
    tx_buf[5] = 0x00;
    calc_sum = tx_buf[1] + tx_buf[2] + tx_buf[3] + tx_buf[4] + tx_buf[5];
    calc_sum %= 0x100;
    tx_buf[6] = calc_sum;
}

void PELCO_D::Left(unsigned char speed)
{
    int calc_sum = 0;
    tx_buf[2] = 0x00;
    tx_buf[3] = 0x04;
    tx_buf[4] = speed;
    tx_buf[5] = 0x00;
    calc_sum = tx_buf[1] + tx_buf[2] + tx_buf[3] + tx_buf[4] + tx_buf[5];
    calc_sum %= 0x100;
    tx_buf[6] = calc_sum;
}

void PELCO_D::Parse()
{
    if (this->rx_buf[0] == 0xee)
    {
        this->online = 1;
        this->Zoom_Ratio = this->rx_buf[1];
        this->Pitch_deg = (this->rx_buf[2] + this->rx_buf[3] * 256) * 0.02197265625;
        this->Yaw_deg = (this->rx_buf[4] + this->rx_buf[5] * 256) * 0.02197265625;
    }
    else
    {
        this->online = 0;
    }
}

void PELCO_D::ZoomOut(unsigned char speed)
{
    int calc_sum = 0;
    tx_buf[2] = 0x00;
    tx_buf[3] = 0x40;
    tx_buf[4] = speed-1;
    tx_buf[5] = 0x00;
    calc_sum = tx_buf[1] + tx_buf[2] + tx_buf[3] + tx_buf[4] + tx_buf[5];
    calc_sum %= 0x100;
    tx_buf[6] = calc_sum;
}

void PELCO_D::ZoomIn(unsigned char speed)
{
    int calc_sum = 0;
    tx_buf[2] = 0x00;
    tx_buf[3] = 0x20;
    tx_buf[4] = speed-1;
    tx_buf[5] = 0x00;
    calc_sum = tx_buf[1] + tx_buf[2] + tx_buf[3] + tx_buf[4] + tx_buf[5];
    calc_sum %= 0x100;
    tx_buf[6] = calc_sum;
}

void PELCO_D::ZoomStop()
{
    int calc_sum = 0;
    tx_buf[2] = 0x00;
    tx_buf[3] = 0x60;
    tx_buf[4] = 0x00;
    tx_buf[5] = 0x00;
    calc_sum = tx_buf[1] + tx_buf[2] + tx_buf[3] + tx_buf[4] + tx_buf[5];
    calc_sum %= 0x100;
    tx_buf[6] = calc_sum;
}
