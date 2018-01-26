//-------------------------------------------------------------------------
// gbn.fh Geosoft Oasis binary file structure.
//-------------------------------------------------------------------------
//
// An Oasis database is an object oriented data structure that is too
// complex to create without the use of the Geosoft object libraries.
// The Oasis binary file is a simple file structure that is intended
// to be used by programmers as a convenient way to get data into an
// Oasis database. The Oasis GBN GX will import Oasis binary files
// into an Oasis database.
//
// A Oasis binary data file consists of an ASCII comment header
// followed by an ASCII 26 (hex <1A>), followed by binary data.
//
// Binary data consists of a series of variable length binary records.
// Each record begins with a single byte value that identifies the record
// type, followed by the record data and data in the case of data records.
//
// The length of each record depends on the record type.
// The length of a data record will be the length of the data record
// structure plus the length of the data.
//
// IEEE numbers and Intel byte order is required.
//
// The first 17 characters of the comment header must be:
// "Oasis BINARY DATA"
//
// Any amount of ASCII text may follow. Oasis will start reading binary
// data at the first byte after the first <1A> byte.
//
// The file structure is as follows:
// Oasis BINARY DATA // required first 17 bytes.
// comment text // As much ASCII text as
// . // desired can be placed
// . // in the comment header.
// . //
// <1A> // end of ASCII comments.
// <01> 80 byte channel record // GBN_ChanRec structure.
// <01> 80 byte channel record // one for each channel of data.
// .
// .
// .
// <02> 28 byte line record // GBN_LineRec structure.
// <03> data record // GBN_Data structure + data
// <03> data record // GBN_Data structure + data
// .
// .
// .
// <02> 28 byte line record // GBN_LineRec structure.
// <03> data record // GBN_Data structure + data
// <03> data record // GBN_Data structure + data
// .
// .
// .
// <00> // end of data.
//
// Notes:
//
// Data records are related to the channels by channel number. If
// you have defined three channels, they will be numbered 0, 1 and 2.
// Channel data records can be in any order(i.e. 0,1,2 or 1,0,2) and
// you do not need to specify all channels on every line.
//
// If the channels already exist in the database, the channel parameters
// are ignored.
//
//------------------------------------------------------------------------
//
// Example:
//
// Hex values are in <hex>, other binary values are in (value,value,...).
//
// The binary type of each value can be determined from the record
// structures. For example, a channel data record (1,5,4610.0,0.1,42150)
// in binary would be:
//
// lChanNumber = 1; // long, 4 bytes
// lBinaryType = 5; // long, 4 bytes
// dFidStart = 4610.0; // double, 8 bytes
// dFidIncrement = 0.1; // double, 8 bytes
// lLength = 42150; // long, 4 bytes
//
// In this example, 42150 double (8-byte) values must immediately
// follow the channel data record.
//
// A carriage-return line-feed <0D0A> is assumed at the end of each
// comment header line for the purpose of calculating offsets.
//
// Channel names are null terminated in a 64 character field.
//
// byte
// offset data comments
// ------ [--------------------------------------------------]------------
// 0 [Oasis BINARY DATA ] required
// 19 [ ]
// 21 [ This is a sample Oasis binary data file which ]
// 69 [ contains two lines, 100 and 110, from flight 10 ]
// 119 [ flown on January 19, 1995. The data includes X, ] comments
// 170 [ Y and GPS time sampled once a second, and Mag and]
// 222 [ single frequency in-phase (EM-I) and quadrature ]
// 272 [ (EM-Q) data sampled 10 times per second. ]
// 315 [ ]
// 317 [<1A> ] start data
// 318 [<01>("Time",4,2,10,1) ] Time chan.
// 399 [<01>("X",5,0,12,1) ] X chan.
// 480 [<01>("Y",5,0,12,1) ] Y chan.
// 561 [<01>("Mag",4,0,10,1) ] Mag chan.
// 642 [<01>("EM_I",4,0,10,0) ] EM inphase
// 723 [<01>("EM_Q",4,0,10,0) ] EM quad.
// 804 [<02>(100,0,0,10,1995,1,19) ] line 100
// 833 [<03>(0,4,1000.0,1.0,3610) (data,data, 3610 times) ] Time data
// 15302 [<03>(1,5,1000.0,1.0,3610) (data,data, 3610 times) ] X data
// 44211 [<03>(2,5,1000.0,1.0,3610) (data,data, 3610 times) ] Y data
// 72120 [<03>(3,4,1000.0,0.1,36100)(data,data, 36100 times)] Mag data
// 216549 [<03>(4,4,1000.0,0.1,36100)(data,data, 36100 times)] EM I data
// 360978 [<03>(5,4,1000.0,0.1,36100)(data,data, 36100 times)] EM Q data
// 505407 [<02>(110,0,0,10,1995,1,19) ] line 110
// 505436 [<03>(0,4,4610.0,1.0,4215) (data,data, 4215 times) ] Time data
// 522325 [<03>(1,5,4610.0,1.0,4215) (data,data, 4215 times) ] X data
// 556074 [<03>(2,5,4610.0,1.0,4215) (data,data, 4215 times) ] Y data
// 589823 [<03>(3,4,4610.0,0.1,42150)(data,data, 42150 times)] Mag data
// 758452 [<03>(4,4,4610.0,0.1,42150)(data,data, 42150 times)] EM I data
// 927081 [<03>(5,4,4610.0,0.1,42150)(data,data, 42150 times)] EM Q data
//1095710 [<00> ] end data
//------------------------------------------------------------------------



#ifndef GBN_FH	// (START A)

#define GBN_FH

// --- record byte headers ---
#define GBN_CHAN_REC 1
#define GBN_LINE_REC 2
#define GBN_DATA_REC 3
#define GBN_EOF_REC 0

// --- establish Geosoft defines if not already set ---
#ifndef LINE_NORMAL	// (START B)

// --- line type defines ---
#define LINE_NORMAL 0
#define LINE_BASE 1
#define LINE_TIE 2
#define LINE_TEST 3
#define LINE_TREND 4
#define LINE_SPECIAL 5
#define LINE_RANDOM 6

// --- data types ---
#define GS_BYTE 0 // signed byte
#define GS_USHORT 1 // unsigned 2-byte integer
#define GS_SHORT 2 // signed 2-byte integer
#define GS_LONG 3 // signed 4-byte integer
#define GS_FLOAT 4 // 4-byte floating point
#define GS_DOUBLE 5 // 8-byte floating point

// A string type is indicated by the negative string length. For example,
// a 10 byte string would be type -10. String data should be NULL
// terminated.

// --- dummy values ---
#define GS_U1DM (char ) 255U
#define GS_S1DM (signed char ) -127
#define GS_U2DM (unsigned short) 65535U
#define GS_S2DM (short ) -32767
#define GS_S4DM (long ) -2147483647L
#define GS_R8DM (double ) -1.0E32

// --- float dummies require a variable to prevent compiler problems ---
#ifndef GS_R4DM		// (START C)
float GBN_r4dm = (float)GS_R8DM
#define GS_R4DM (GBN_r4dum)
#endif				// (END C)

// --- display formats ---
#define GSF_NORMAL 0 // Normal decimal number format
#define GSF_EXP 1 // Exponential floating point (1.2E+23)
#define GSF_TIME 2 // Time (HH:MM:SS.SSSS)
#define GSF_DATE 3 // Date (YYYY/MM/DD)
#define GSF_GGRAPH 4 // Geographical (DEG.MM.SS.SSS)

// --- end of defines ---
#endif	// (END B)


// --- line record ---
typedef struct {
	long lLineNumber;
	long lLineVersion;
	long lLineType; // one of LINE_? 
	long lFlight;
	long lYear;
	long lMonth;
	long lDay;
} GBN_LineRec;

// --- channel record ---
typedef struct {
	char szName[64]; 	// channel name, case tolerant, 
						// NULL terminated 
						// -------------------------------------------------------------------- 
						// If the channel does not already exist, the following 4 parameters 
						// are used to create the Oasis channel structure. If the channel 
						// exists, this information is ignored. 
						// -------------------------------------------------------------------- 
	long lChannelType; // channel data type, one of GS_? 
	long lDisplayFormat; // one of GSF_? 
	long lDisplayWidth; // display width in characters 
	long lDisplayDecimals; // digits after decimal place 
} GBN_ChanRec;


// --- data record ---
typedef struct {
	long lChanNumber; // number from channel list, 0 is first 
	long lBinaryType; // incoming binary data type, one of GS_? 
	double dFidStart; // start fiducial number 
	double dFidIncrement; // fiducial increment 
	long lLength; // number of data elements that follow 
} GBN_Data;

#endif 		// (END A)