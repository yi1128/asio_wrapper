#pragma once



class asio_wrapper
{
public:
	asio_wrapper();
	~asio_wrapper();
	
	/// <summary>
	/// Initialize of Network
	/// IP, Port
	/// </summary>
	/// <returns></returns>
	virtual bool Init();

	
private:

	

};

class asio_TCP_wrapper : public asio_wrapper
{
public:
	
private:

};

class asio_UDP_wrapper : public asio_wrapper
{
public:

private:

};