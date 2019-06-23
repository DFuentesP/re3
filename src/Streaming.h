#pragma once

enum {
	STREAM_OFFSET_MODEL = 0,
	STREAM_OFFSET_TXD = STREAM_OFFSET_MODEL+MODELINFOSIZE,
	NUMSTREAMINFO = STREAM_OFFSET_TXD+TXDSTORESIZE
};

enum StreamFlags
{
	STREAMFLAGS_DONT_REMOVE = 0x01,
	STREAMFLAGS_SCRIPTOWNED = 0x02,
	STREAMFLAGS_DEPENDENCY  = 0x04,
	STREAMFLAGS_PRIORITY    = 0x08,
	STREAMFLAGS_NOFADE      = 0x10,

	STREAMFLAGS_NOT_IN_LIST = STREAMFLAGS_DONT_REMOVE|STREAMFLAGS_SCRIPTOWNED,
	STREAMFLAGS_KEEP_IN_MEMORY = STREAMFLAGS_DONT_REMOVE|STREAMFLAGS_SCRIPTOWNED|STREAMFLAGS_DEPENDENCY,
};

enum StreamLoadState
{
	STREAMSTATE_NOTLOADED = 0,
	STREAMSTATE_LOADED    = 1,
	STREAMSTATE_INQUEUE   = 2,
	STREAMSTATE_READING   = 3,	// what is this?
	STREAMSTATE_STARTED   = 4,	// first part read
};

enum ChannelState
{
	CHANNELSTATE_0 = 0,
};

class CStreamingInfo
{
public:
	CStreamingInfo *m_next;
	CStreamingInfo *m_prev;
	uint8 m_loadState;
	uint8 m_flags;

	int16  m_nextID;
	uint32 m_position;
	uint32 m_size;

	bool GetCdPosnAndSize(uint32 &posn, uint32 &size);
	void SetCdPosnAndSize(uint32 posn, uint32 size);
	void AddToList(CStreamingInfo *link);
	void RemoveFromList(void);
	uint32 GetCdSize(void) { return m_size; }
};

struct CStreamingChannel
{
	int32 modelIds[4];
	int32 offsets[4];
	int32 state;
	int32 field24;
	int32 position;
	int32 size;
	int32 field30;
	int32 status;	// from CdStream
};

class CDirectory;
enum eLevelName;

class CStreaming
{
public:
	static bool &ms_disableStreaming;
	static bool &ms_bLoadingBigModel;
	static int32 &ms_numModelsRequested;
	static CStreamingInfo *ms_aInfoForModel;	//[NUMSTREAMINFO]
	static CStreamingInfo &ms_startLoadedList;
	static CStreamingInfo &ms_endLoadedList;
	static CStreamingInfo &ms_startRequestedList;
	static CStreamingInfo &ms_endRequestedList;
	static int32 &ms_oldSectorX;
	static int32 &ms_oldSectorY;
	static uint32 &ms_streamingBufferSize;
	static uint8 **ms_pStreamingBuffer;	//[2]
	static int32 &ms_memoryUsed;
	static CStreamingChannel *ms_channel;	//[2]
	static int32 &ms_numVehiclesLoaded;
	static int32 *ms_vehiclesLoaded;	//[MAXVEHICLESLOADED]
	static CDirectory *&ms_pExtraObjectsDir;
	static int32 &ms_numPriorityRequests;
	static bool &ms_hasLoadedLODs;
	static int32 &ms_currentPedGrp;
	static int32 ms_lastCullZone;
	static uint16 &ms_loadedGangs;
	static int32 ms_currentPedLoading;
	static int32 *ms_imageOffsets;	//[NUMCDIMAGES]
	static int32 &ms_lastImageRead;
	static int32 &ms_imageSize;
	static int32 &ms_memoryAvailable;

	static void Init(void);
	static void Shutdown(void);
	static void LoadCdDirectory(void);
	static void LoadCdDirectory(const char *dirname, int32 n);
	static bool ConvertBufferToObject(int8 *buf, int32 streamId);
	static bool FinishLoadingLargeFile(int8 *buf, int32 streamId);
	static void RequestModel(int32 model, int32 flags);
	static void RequestTxd(int32 txd, int32 flags) { RequestModel(txd + STREAM_OFFSET_TXD, flags); }
	static void RequestSubway(void);
	static void RequestBigBuildings(eLevelName level);
	static void RequestIslands(eLevelName level);
	static void RequestSpecialModel(int32 modelId, const char *modelName, int32 flags);
	static void RequestSpecialChar(int32 charId, const char *modelName, int32 flags);
	static void RemoveModel(int32 id);
	static void RemoveTxd(int32 id) { RemoveModel(id + STREAM_OFFSET_TXD); }

	static bool IsTxdUsedByRequestedModels(int32 txdId);
	static bool AddToLoadedVehiclesList(int32 modelId);

	static void MakeSpaceFor(int32 size);
	static void ImGonnaUseStreamingMemory(void);
	static void IHaveUsedStreamingMemory(void);
	static void UpdateMemoryUsed(void);
};
