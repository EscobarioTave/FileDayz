class VPPNewsFeedDataManager
{
	private ref array<ref VPPNewsElement> m_Elements;
	private bool m_ContentLoaded;

	void VPPNewsFeedDataManager()
	{
		m_Elements = new array<ref VPPNewsElement>;
		GetNewsFeedData();
	}

	//Called by NewsFeedCB when a valid response occurs
	void LoadData()
	{
		if (!FileExist("$profile:VPPNewsFeed.json"))
			GetNewsFeedData();

		JsonFileLoader<VPPNewsFeedDataManager>.JsonLoadFile("$profile:VPPNewsFeed.json", this);
	}

	void GetNewsFeedData()
	{
		//Get Data usin curl
		NewsFeedCB cb = new NewsFeedCB(this);
		RestContext ctx = CreateRestApi().GetRestContext("https://www.dayzvpp.com");
		ctx.GET( cb, "/vpp-news-feed" );
	}

	ref array<ref VPPNewsElement> GetElements()
	{
		return m_Elements;
	}

	void Setloaded(bool state)
	{
		m_ContentLoaded = state;
	}

	bool ContentLoaded()
	{
		return m_ContentLoaded;
	}
};