class NewsFeedCB: RestCallback
{
	private ref VPPNewsFeedDataManager rootClass;

	void NewsFeedCB(VPPNewsFeedDataManager root)
	{
		rootClass = root;
	}

	override void OnError(int errorCode)
	{
		Print("NewsFeedCB: Error Code -> "+ errorCode);
	}
	
	override void OnSuccess( string data, int dataSize )
	{
		FileHandle file = OpenFile("$profile:VPPNewsFeed.json", FileMode.WRITE);
		if (file != 0)
		{
			FPrint(file, data);
			CloseFile(file);
		}

		if (rootClass != null)
		{
			rootClass.LoadData(); //Make callback to parent and load files once https req is done
			rootClass.Setloaded(true);
		}
	}
};