class UserInterfaceC
{
public:
	static UserInterfaceC	*CreateInstance();
	static UserInterfaceC	*GetInstance() {return sInstance;};
	~UserInterfaceC(){};
	
	void		render();

    void        rightMousePressed();

private:
	static void drawString(uint32_t x, uint32_t y, char* format, ...);
	static void drawStringNoRaster(char* format, ...);

	static UserInterfaceC			*sInstance;
	UserInterfaceC(){};
};