/* Copyright IBM Corp. 2013, 2019 */

#pragma pack(packed)
struct infbk
{
  long int infresp;		/* Minimum response buffer size      @VRLDWDN */
};
/* Values for field "infresp" */
#define infrcok  0		/* Success                           @U7128DN */
#define infrfc   4		/* Unsupported function code         @U7128DN */
#define infrcaut 8		/* Unauthorized function code        @VRLDWDN */
#define infrcmis 12		/* Missing or invalid name           @VRLDWDN */
#define infrcnam 16		/* Specified name is unknown         @VRLDWDN */
#define infrcbuf 20		/* Response buffer is too small      @VRLDWDN */
#define infrcnxt 24		/* Next, unused return code          @VRLDWDN */
#define infcapac 0		/* FC=0 - Obtain CPU Capacity Info   @U7128DN */
#define infhyper 1		/* FC=1 - Hypervisor Environment Info@VRLDWDN */
#define infgstl  2		/* FC=2 - Guest List                 @VRLDWDN */
#define infgsti  3		/* FC=3 - Designated Guest Info      @VRLDWDN */
#define infpooll 4		/* FC=4 - Resource Pool List         @VRLDWDN */
#define infpooli 5		/* FC=5 - Designated Resource Pool            */
#define infpoolm 6		/* FC=6 - Resource Pool Member List  @VRLDWDN */
struct inf0hdr
{
  unsigned char infhflg1;	/* Header Flag Byte 1                         */
  unsigned char infhflg2;	/* Header Flag Byte 2 reserved for IBM use    */
  unsigned char infhval1;	/* Header Validity Byte 1                     */
  unsigned char infhval2;	/* Header Validity Byte 2                     */
  unsigned char _filler1[3];	/* Reserved for IBM use              @VRLDWDN */
  char infhygct;		/* Count of reported hypervisors/             */
  short int infhtotl;		/* Total length of the response               */
  short int infhdln;		/* Length of Header Section in bytes          */
  short int infmoff;		/* Offset to Machine Section mapped           */
  short int infmlen;		/* Length of Machine Section in bytes         */
  short int infpoff;		/* Offset to Partition Section mapped         */
  short int infplen;		/* Length of Partition Section in bytes       */
  union
  {
    long int infhygs1;		/* Hypervisor/Guest Header 1        @VRLDWDN */
    struct
    {
      short int infhoff1;	/* Offset to Hypervisor Section 1,            */
      short int infhlen1;	/* Length of Hypervisor Section 1 in          */
      short int infgoff1;	/* Offset to Guest Section 1 mapped           */
      short int infglen1;	/* Length of Guest Section 1 in bytes         */
    };
  };
  union
  {
    long int infhygs2;		/* Hypervisor/Guest Header 2        @VRLDWDN */
    struct
    {
      short int infhoff2;	/* Offset to Hypervisor Section 2             */
      short int infhlen2;	/* Length of Hypervisor Section 2 in          */
      short int infgoff2;	/* Offset to Guest Section 2 mapped           */
      short int infglen2;	/* Length of Guest Section 2 in bytes         */
    };
  };
  union
  {
    long int infhygs3;		/* Hypervisor/Guest Header 3        @VRLDWDN */
    struct
    {
      short int infhoff3;	/* Offset to Hypervisor Section 3             */
      short int infhlen3;	/* Length of Hypervisor Section 3 in          */
      short int infgoff3;	/* Offset to Guest Section 3 mapped           */
      short int infglen3;	/* Length of Guest Section 3 in bytes         */
    };
  };
  unsigned char _filler2[4];	/* Reserved for IBM use              @VRLDWDN */
};
/* Values for field "infhflg1" */
#define infgpdu  0x80		/* Global Performance Data unavailable        */
#define infsthyi 0x40		/* One or more hypervisor levels              */
#define infvsi   0x20		/* Virtualization stack is incomplete.        */
#define infbasic 0x10		/* Execution environment is not               */
/* Values for field "infhygct" */
#define inf0ygmx 3		/* Maximum Hypervisor/Guest sections @VRLDWDN */
/* Values for field "_filler2" */
#define inf0hdsz 0x30		/* Size of header in bytes                    */
#define inf0hdsd 0x06		/* Size of Header in doublewords   @VRLDWDN   */
struct inf0hdyg
{
  short int infyoff;		/* Offset to Hypervisor Section               */
  short int infylen;		/* Length of Hypervisor Section in            */
  short int infgoff;		/* Offset to Guest Section mapped             */
  short int infglen;		/* Length of Guest Section in bytes           */
};
/* Values for field "infglen" */
#define inf0hysz 0x08		/* Size of section description in bytes    */
#define inf0hysd 0x01		/* Size of section description in          */
struct inf0mac
{
  unsigned char infmflg1;	/* Machine Flag Byte 1               @VRLRXDN */
  unsigned char infmflg2;	/* Machine Flag Byte 2 reserved               */
  unsigned char infmval1;	/* Machine Validity Byte 1                    */
  unsigned char infmval2;	/* Machine Validity Byte 2                    */
  short int infmscps;		/* Count of shared CPs configured             */
  short int infmdcps;		/* Count of dedicated CPs configured          */
  short int infmsifl;		/* Count of shared IFLs configured            */
  short int infmdifl;		/* Count of dedicated IFLs configured         */
  unsigned char infmname[8];	/* Machine Name, in EBCDIC format.            */
  unsigned char infmtype[4];	/* Machine Type, in EBCDIC format.            */
  unsigned char infmmanu[16];	/* Machine Manufacturer, in EBCDIC            */
  unsigned char infmseq[16];	/* Sequence Code, in EBCDIC format.           */
  unsigned char infmpman[4];	/* Plant of Manufacture, in EBCDIC            */
  unsigned char _filler1[4];	/* Reserved for IBM use              @VRLDWDN */
  unsigned char infmplnm[8];	/* Reserved for IBM use.             @Y1302DN */
  short int infmsziip;		/* Count of shared zIIPs configured           */
  short int infmdziip;		/* Count of dedicated zIIPs configured        */
  unsigned char _filler2[4];	/* Reserved for IBM use              @VRLDXDN */
};
/* Values for field "infmflg1" */
#define infmpool  0x80		/* Reserved for IBM use.             @Y1302DN */
/* Values for field "infmval1" */
#define infmproc  0x80		/* Processor Count Validity                   */
#define infmmid   0x40		/* Machine ID Validity                        */
#define infmmnam  0x20		/* Machine Name Validity                      */
#define infmplnv  0x10		/* Reserved for IBM use.             @Y1302DN */
#define infmziipv 0x08		/* Machine zIIP reporting validity.           */
/* Values for field "_filler2" */
#define inf0msiz  0x50		/* Size of Machine Section in bytes           */
#define inf0mszd  0x0A		/* Size of Machine Section in                 */
struct inf0par
{
  unsigned char infpflg1;	/* Partition Flag Byte 1             @VRKP30K */
  unsigned char infpflg2;	/* Partition Flag Byte 2 reserved for         */
  unsigned char infpval1;	/* Partition Validity Byte 1                  */
  unsigned char infpval2;	/* Partition Validity Byte 2 reserved         */
  short int infppnum;		/* Logical Partition Number. This is          */
  short int infpscps;		/* Count of shared logical CP cores  @VRKP30K */
  short int infpdcps;		/* Count of dedicated logical CP cores        */
  short int infpsifl;		/* Count of shared logical IFL cores @VRKP30K */
  short int infpdifl;		/* Count of dedicated logical IFL cores       */
  unsigned char _filler1[2];	/* Reserved for IBM use              @VRLDWDN */
  unsigned char infppnam[8];	/* Logical Partition Name, in EBCDIC          */
  int infpwbcp;			/* Partition weight-based capped              */
  int infpabcp;			/* Partition absolute capped                  */
  int infpwbif;			/* Partition weight-based capped              */
  int infpabif;			/* Partition absolute capped                  */
  unsigned char infplgnm[8];	/* LPAR Group Name (Valid if INFPLGVL)        */
  int infplgcp;			/* LPAR Group Absolute Capacity               */
  int infplgif;			/* LPAR Group Absolute Capacity               */
  unsigned char infpplnm[8];	/* Reserved for IBM use.             @Y1302DN */
  short int infpsziip;		/* Count of shared logical zIIP cores         */
                                /* configured for this partition.             */
                                /* (Valid if INFPZIIPV)                       */
  short int infpdziip;		/* Count of dedicated logical zIIP cores      */
                                /*configured for this partition.              */
                                /* (Valid if INFPZIIPV)                       */
  int infpwbziip;		/* Partition weight-based capped              */
                                /* capacity for zIIPs, a scaled               */
                                /* number where X'00010000'                   */
                                /* represents one core.                       */
                                /* Cap is applicable only to shared           */
                                /* processors. Zero if not capped.            */
                                /* (Valid if INFPWBCC + INFPZIIPV)            */
  int infpabziip;		/* Partition absolute capped                  */
                                /* capacity for zIIPs, a scaled               */
                                /* number where X'00010000'                   */
                                /* represents one core.                       */
                                /* Cap is applicable only to shared           */
                                /* processors. Zero if not capped.            */
                                /* (Valid if INFPACC + INFPZIIPV)             */
  int infplgziip;		/* LPAR Group Absolute Capacity               */
                                /* Value for the zIIP CPU type                */
                                /* when nonzero. Nonzero only when            */
                                /* INFPLGNM is nonzero and a cap is           */
                                /* defined for this LPAR Group for            */
                                /* the zIIP CPU type. When nonzero,           */
                                /* contains a scaled number where             */
                                /* X'00010000' represents one core.           */
                                /* (Valid if INFPLGVL + INFPZIIPV)            */
};
/* Values for field "infpflg1" */
#define infpmten  0x80		/* Multithreading (MT) is enabled.            */
#define infppool  0x40		/* Reserved for IBM use.             @Y1302DN */
/* Values for field "infpval1" */
#define infpproc  0x80		/* Processor Count Validity                   */
                                /* This bit being on indicates that           */
                                /* INFPSCPS, INFPDCPS, INFPSIFL,              */
                                /* and INFPDIFL contain valid counts.         */
#define infpwbcc  0x40		/* Partition weight-based capped              */
                                /* capacity validity.                         */
                                /* This bit being on indicates that           */
                                /* INFPWBCP and INFPWBIF are valid.           */
#define infpacc   0x20		/* Partition absolute capped capacity         */
                                /* validity.                                  */
                                /* This bit being on indicates that           */
#define infppid   0x10		/* Partition ID Validity.                     */
                                /* This bit being on indicates that           */
                                /* a SYSIB 2.2.2 was obtained from            */
                                /* STSI and information reported in           */
                                /* the following fields is valid:             */
                                /* INFPPNUM, INFPPNAM                         */
#define infplgvl  0x08		/* LPAR Group Absolute Capacity               */
                                /* Capping information validity               */
                                /* This bit being on indicates                */
                                /* that INFPLGNM, INFPLGCP and                */
                                /* INFPLGIF are valid.                        */
#define infpplnv  0x04		/* Reserved for IBM use.             @Y1302DN */
#define infpziipv 0x02		/* Partition zIIP reporting validity.         */
                                /* When on, INFPSZIIP, INFPDZIIP,             */
                                /* INFPWBZIIP, INFPABZIIP and                 */
                                /* INFPLGZIIP fields are valid.               */
#define infpmtiv  0x01		/* INFPMTIV=1 indicates data is not           */
/* Values for field "infplgziip" */
#define inf0psiz  0x50		/* Size of Partition Section in bytes         */
#define inf0pszd  0x0A		/* Size of Partition Section in               */
struct inf0hyp
{
  unsigned char infyflg1;	/* Hypervisor Flag Byte 1            @U7105P4 */
  unsigned char infyflg2;	/* Hypervisor Flag Byte 2                     */
  unsigned char infyval1;	/* Hypervisor Validity Byte 1        @VRLDXDN */
  unsigned char infyval2;	/* Hypervisor Validity Byte 2                 */
  char infytype;		/* Hypervisor type                   @VRLDWDN */
  unsigned char _filler1;	/* Reserved for IBM use              @VRLDWDN */
  unsigned char infycpt;	/* Threads in use per CP core.       @VRKP30K */
  unsigned char infyiflt;	/* Threads in use per IFL core.      @VRKP30K */
  unsigned char infysyid[8];	/* System Identifier, in EBCDIC               */
  unsigned char infyclnm[8];	/* Cluster Name, in EBCDIC format,            */
  short int infyscps;		/* Number of CP cores shared by      @VRKP30K */
  short int infydcps;		/* Number of CP cores dedicated      @VRKP30K */
  short int infysifl;		/* Number of IFL cores shared by     @VRKP30K */
  short int infydifl;		/* Number of IFL cores dedicated     @VRKP30K */
  struct
  {
    unsigned char infyins0;	/* Function codes 0-7.               @VRLDWDN */
    unsigned char infyins1;	/* Function codes 8-15.              @VRLDWDN */
    unsigned char infyins2;	/* Function codes 16-23.             @VRLDWDN */
    unsigned char infyins3;	/* Function codes 24-31.             @VRLDWDN */
    unsigned char infyins4;	/* Function codes 32-39.             @VRLDWDN */
    unsigned char infyins5;	/* Function codes 40-47.             @VRLDWDN */
    unsigned char infyins6;	/* Function codes 48-55.             @VRLDWDN */
    unsigned char infyins7;	/* Function codes 56-63.             @VRLDWDN */
  } infyinsf;
  struct
  {
    unsigned char infyaut0;	/* Function codes 0-7.               @VRLDWDN */
    unsigned char infyaut1;	/* Function codes 8-15.              @VRLDWDN */
    unsigned char infyaut2;	/* Function codes 16-23.             @VRLDWDN */
    unsigned char infyaut3;	/* Function codes 24-31.             @VRLDWDN */
    unsigned char infyaut4;	/* Function codes 32-39.             @VRLDWDN */
    unsigned char infyaut5;	/* Function codes 40-47.             @VRLDWDN */
    unsigned char infyaut6;	/* Function codes 48-55.             @VRLDWDN */
    unsigned char infyaut7;	/* Function codes 56-63.             @VRLDWDN */
  } infyautf;
  unsigned char infyziipt;	/* Threads in use per zIIP core.              */
  unsigned char _filler2;	/* Reserved for IBM use.             @VRLDXDN */
  short int infysziip;		/* Number of zIIP cores shared by             */
  int _filler3;			/* Reserved for IBM use.             @VRLDXDN */
};
/* Values for field "infyflg1" */
#define infylmcn  0x80		/* Consumption method is used to              */
#define infylmpr  0x40		/* If on, Limithard caps use                  */
#define infymten  0x20		/* Hypervisor is MT-enabled.         @VRLDWDN */
/* Values for field "infyval1" */
#define infyziipv 0x80		/* Hypervisor zIIP reporting validity.        */
#define infymtiv  0x40		/* INFYMTIV=1 indicates data is not           */
/* Values for field "infytype" */
#define infytvm   0x01		/* z/VM is the hypervisor                     */
#define infytkvm  0x02		/* KVM is the hypervisor             @VRLDXDN */
#define infytzcx  0x03		/* zCX is the hypervisor             @VRLDXDN */
/* Values for field "infyins0" */
#define infyfccp  0x80		/* FC=0 - Obtain CPU Capacity Info.  @VRLDWDN */
#define infyfhyp  0x40		/* FC=1 - Hypervisor Environment Info@VRLDWDN */
#define infyfgls  0x20		/* FC=2 - Guest List                 @VRLDWDN */
#define infyfgst  0x10		/* FC=3 - Designated Guest Info      @VRLDWDN */
#define infyfpls  0x08		/* FC=4 - Resource Pool List         @VRLDWDN */
#define infyfpds  0x04		/* FC=5 - Designated Resource Pool            */
#define infyfpml  0x02		/* FC=6 - Resource Pool Member List  @VRLDWDN */
/* Values for field "_filler3" */
#define inf0ysiz  0x38		/* Size of Hypervisor Section in bytes        */
#define inf0yszd  0x07		/* Size of Hypervisor Section in              */
struct inf0gst
{
  unsigned char infgflg1;	/* Guest Flag Byte 1                          */
  unsigned char infgflg2;	/* Guest Flag Byte 2                          */
  unsigned char infgval1;	/* Guest Validity Byte 1             @VRLDXDN */
  unsigned char infgval2;	/* Guest Validity Byte 2                      */
  unsigned char infgusid[8];	/* Guest's userid, in EBCDIC format           */
  short int infgscps;		/* Number of guest shared CPs                 */
  short int infgdcps;		/* Number of guest dedicated CPs              */
  unsigned char infgcpdt;	/* Dispatch type for guest CPs                */
  unsigned char _filler1[3];	/* Reserved for IBM use              @VRLDWDN */
  int infgcpcc;			/* Guest current capped capacity for          */
  short int infgsifl;		/* Number of guest shared IFLs                */
  short int infgdifl;		/* Number of guest dedicated IFLs             */
  unsigned char infgifdt;	/* Dispatch type for guest IFLs. This         */
  unsigned char _filler2[3];	/* Reserved for IBM use              @VRLDWDN */
  int infgifcc;			/* Guest current capped capacity for          */
  unsigned char infgpflg;	/* Resource Pool Capping Flags       @VRLDWDN */
  unsigned char _filler3[3];	/* Reserved for IBM use              @VRLDWDN */
  unsigned char infgpnam[8];	/* Resource pool name. Blanks if not          */
  int infgpccc;			/* Resource pool capped capacity for          */
  int infgpicc;			/* Resource pool capped capacity for          */
  short int infgsziip;		/* Number of guest shared zIIPs.              */
  unsigned char infgziipdt;	/* Dispatch type for guest zIIPs. This        */
  unsigned char _filler4;	/* Reserved for IBM use.             @VRLDXDN */
  int infgziipcc;		/* Guest current capped capacity for          */
  int infgpzcc;			/* Resource pool capped capacity for          */
  int _filler5;			/* Reserved for IBM use.             @VRLDXDN */
};
/* Values for field "infgflg1" */
#define infgmob    0x80		/* Guest mobility is enabled.        @U7405P4 */
#define infgmcpt   0x40		/* Guest has multiple CPU types               */
#define infgcplh   0x20		/* Guest CP dispatch type has                 */
#define infgiflh   0x10		/* Guest IFL dispatch type has                */
#define infgvcpt   0x08		/* Virtual CPs are thread dispatched.         */
#define infgvift   0x04		/* Virtual IFLs are thread dispatched         */
#define infgziiph  0x02		/* Guest zIIP dispatch type has               */
#define infgvziipt 0x01		/* Virtual zIIPs are thread dispatched.       */
/* Values for field "infgval1" */
#define infgziipv  0x80		/* Guest zIIP reporting validity.             */
#define infgmtiv   0x40		/* INFGMTIV=1 indicates data is not           */
/* Values for field "infgifdt" */
#define infgpuccp  0x00		/* General Purpose (CP)              @VRLDWDN */
#define infgpucifl 0x03		/* Integrated Fac for Linux (IFL).   @VRLDWDN */
#define infgpuczip 0x05		/* zSeries Integrated Information             */
#define infgpuczcp 0xFF		/* May be dispatched on zIIP and CP           */
/* Values for field "infgpflg" */
#define infgpclh   0x80		/* Resource Pool's CP virtual type            */
#define infgpcpc   0x40		/* Resource Pool's CP virtual type            */
#define infgpilh   0x20		/* Resource Pool's IFL virtual type           */
#define infgpifc   0x10		/* Resource Pool's IFL virtual type           */
#define infprctm   0x08		/* Resource Pool uses prorated core           */
#define infgpzlh   0x04		/* Resource Pool's zIIP virtual type          */
#define infgpzpc   0x02		/* Resource Pool's zIIP virtual type          */
/* Values for field "_filler5" */
#define inf0gsiz   0x48		/* Size of Guest Section in bytes             */
#define inf0gszd   0x09		/* Size of Guest Section in                   */
struct infchdr
{
  short int infcvrsn;		/* Version number indicating the              */
  short int infchdln;		/* Length of the full header that             */
  int infctotl;			/* Total length of the actual data            */
  short int infcrqsz;		/* Required buffer size as a number           */
  short int infclsof;		/* Offset to start of list from the           */
  short int infcenln;		/* Length of list entry.             @VRLDWDN */
  short int _filler1;		/* Reserved for IBM use              @VRLDWDN */
  int infccten;			/* Count of list entries.            @VRLDWDN */
  int _filler2;			/* Reserved for IBM use              @VRLDWDN */
  double _filler3[3];		/* Reserved for IBM use              @VRLDWDN */
  double infcfcus[2];		/* Function code-specific section.            */
};
/* Values for field "infcfcus" */
#define infcsb1 0x40		/* Version 1 length in bytes                */
#define infcsd1 0x08		/* Version 1 length in doublewords          */
struct inf1bk
{
  union
  {
    unsigned char inf1chdr[64];	/* Common section of header mapped            */
    struct
    {
      short int inf1vrsn;	/* Response buffer version.        @UB040DN */
      unsigned char _filler1[62];
    };
  };
  unsigned char inf1hfl1;	/* Header Flag Byte 1                         */
  unsigned char inf1hfl2;	/* Reserved for IBM use              @VRLDWDN */
  unsigned char inf1hvl1;	/* Reserved for IBM use              @VRLDWDN */
  unsigned char inf1hvl2;	/* Reserved for IBM use              @VRLDWDN */
  int _filler2;			/* Reserved for IBM use              @VRLDWDN */
  char inf1hygc;		/* Count of reported hypervisors/             */
  short int inf1moff;		/* Offset from start of INF1BK                */
  short int inf1mlen;		/* Length of Machine Section in bytes@VRLDWDN */
  short int inf1poff;		/* Offset from start of INF1BK                */
  short int inf1plen;		/* Len of Partition Section in bytes @VRLDWDN */
  struct
  {
    short int inf1yof1;		/* Offset from start of INF1BK                */
    short int inf1yln1;		/* Length of Hypervisor Section 1 in          */
    short int inf1yvr1;		/* Version number of this hypervisor          */
    short int _filler3;		/* Reserved for IBM use.             @VRLDWDN */
    short int inf1gof1;		/* Offset from start of INF1BK                */
    short int inf1gln1;		/* Length of Guest Section 1 in bytes@VRLDWDN */
    short int inf1gvr1;		/* Version number of this guest               */
    short int _filler4;		/* Reserved for IBM use.             @VRLDWDN */
  } inf1hyg1;
  struct
  {
    short int inf1yof2;		/* Offset from start of INF1BK                */
    short int inf1yln2;		/* Length of Hypervisor Section 2 in          */
    short int inf1yvr2;		/* Version number of this hypervisor          */
    short int _filler5;		/* Reserved for IBM use.             @VRLDWDN */
    short int inf1gof2;		/* Offset from start of INF1BK                */
    short int inf1gln2;		/* Length of Guest Section 2 in bytes@VRLDWDN */
    short int inf1gvr2;		/* Version number of this guest               */
    short int _filler6;		/* Reserved for IBM use.             @VRLDWDN */
  } inf1hyg2;
  struct
  {
    short int inf1yof3;		/* Offset from start of INF1BK                */
    short int inf1yln3;		/* Length of Hypervisor Section 3 in          */
    short int inf1yvr3;		/* Version number of this hypervisor          */
    short int _filler7;		/* Reserved for IBM use.             @VRLDWDN */
    short int inf1gof3;		/* Offset from start of INF1BK                */
    short int inf1gln3;		/* Length of Guest Section 3 in bytes@VRLDWDN */
    short int inf1gvr3;		/* Version number of this guest               */
    short int _filler8;		/* Reserved for IBM use.             @VRLDWDN */
  } inf1hyg3;
};
/* Values for field "inf1vrsn" */
#define inf1v00001 0x01		/* Initial version. APAR VM66105.    @VRLDWDN */
/* Values for field "inf1hfl1" */
#define inf1gpdu   0x80		/* Global Performance Data unavailable.       */
#define inf1sthy   0x40		/* One or more hypervisor levels              */
#define inf1vsi    0x20		/* Virtualization stack is incomplete.        */
#define inf1basc   0x10		/* Execution environment is not               */
#define inf1fcfc   0x08		/* A lower level hypervisor supports          */
#define inf1fcau   0x04		/* A lower level hypervisor supports          */
/* Values for field "inf1hygc" */
#define inf1ygmx   3		/* Maximum Hypervisor/Guest sections @VRLDWDN */
/* Values for field "_filler8" */
#define inf1bsb1   0x80		/* Version 1 length in bytes                  */
#define inf1bsd1   0x10		/* Version 1 length in doublewords            */
struct inf1yghd
{
  short int inf1yoff;		/* Offset from start of INF1BK                */
  short int inf1ylen;		/* Length of Hypervisor Section in            */
  short int inf1yvrs;		/* Version number of this hypervisor          */
  short int _filler1;		/* Reserved for IBM use.             @VRLDWDN */
  short int inf1goff;		/* Offset from start of INF1BK                */
  short int inf1glen;		/* Length of Guest Section in bytes  @VRLDWDN */
  short int inf1gvrs;		/* Version number of this guest               */
  short int _filler2;		/* Reserved for IBM use.             @VRLDWDN */
};
/* Values for field "_filler2" */
#define inf1ygb1 0x10		/* Version 1 length in bytes                */
#define inf1ygd1 0x02		/* Version 1 length in doublewords          */
struct inf1mac
{
  unsigned char inf1mfl1;	/* Machine Flag Byte 1               @VRLDWDN */
  unsigned char inf1mfl2;	/* Machine Flag Byte 2                        */
  unsigned char inf1mvl1;	/* Machine Field Validity Byte 1     @VRLDWDN */
  unsigned char inf1mvl2;	/* Reserved for IBM use              @VRLDWDN */
  short int inf1mscp;		/* Count of shared CPs configured             */
  short int inf1mdcp;		/* Count of dedicated CPs configured          */
  short int inf1msif;		/* Count of shared IFLs configured            */
  short int inf1mdif;		/* Count of dedicated IFLs configured         */
  unsigned char inf1mtyp[4];	/* Machine Type, in EBCDIC format.            */
  unsigned char inf1mnam[8];	/* Machine Name, in EBCDIC format.            */
  unsigned char inf1mman[16];	/* Machine Manufacturer, in EBCDIC            */
  unsigned char inf1mseq[16];	/* Sequence Code, in EBCDIC format.           */
  unsigned char inf1mpma[4];	/* Plant of Manufacture, in EBCDIC            */
  int _filler1;			/* Reserved for IBM use              @VRLDWDN */
  unsigned char inf1mplnm[8];	/* Reserved for IBM use.             @VRLDWDN */
};
/* Values for field "inf1mfl1" */
#define inf1mpool 0x80		/* Reserved for IBM use.             @VRLDWDN */
/* Values for field "inf1mvl1" */
#define inf1mproc 0x80		/* Processor Count Validity                   */
#define inf1midv  0x40		/* Machine ID Validity                        */
#define inf1mnmv  0x20		/* Machine Name Validity                      */
#define inf1mplnv 0x10		/* Reserved for IBM use.             @VRLDWDN */
/* Values for field "inf1mplnm" */
#define inf1msb1  0x48		/* Version 1 length in bytes                  */
#define inf1msd1  0x09		/* Version 1 length in doublewords            */
struct inf1par
{
  unsigned char inf1pfl1;	/* Partition Flag Byte 1             @VRLDWDN */
  unsigned char inf1pfl2;	/* Partition Flag Byte 2 reserved for         */
  unsigned char inf1pvl1;	/* Partition Field Validity Byte 1   @VRLDWDN */
  unsigned char inf1pvl2;	/* Reserved for IBM use              @VRLDWDN */
  short int inf1pscp;		/* Count of shared logical CP cores           */
  short int inf1pdcp;		/* Count of dedicated logical CP cores        */
  short int inf1psif;		/* Count of shared logical IFL cores          */
  short int inf1pdif;		/* Count of dedicated logical IFL cores       */
  short int inf1ppnu;		/* Logical Partition Number. This is          */
  char inf1pcmod;		/* Logical Partition Config Mode.    @VRLDWDN */
  char inf1prcpu;		/* Partition primary CPU type. Valid          */
  unsigned char inf1ppna[8];	/* Logical Partition Name, in EBCDIC          */
  int inf1pwbc;			/* Partition weight-based capped              */
  int inf1pabc;			/* Partition absolute capped                  */
  int inf1pwbi;			/* Partition weight-based capped              */
  int inf1pabi;			/* Partition absolute capped                  */
  unsigned char inf1plgn[8];	/* LPAR Group Name (Valid if INF1PLGV)        */
  int inf1plgc;			/* LPAR Group Absolute Capacity               */
  int inf1plgi;			/* LPAR Group Absolute Capacity               */
  unsigned char inf1pplnm[8];	/* Reserved for IBM use.             @VRLDWDN */
  int inf1penc;			/* Partition Entitled Capacity                */
  int inf1peni;			/* Partition Entitled Capacity                */
  long int inf1psxca;		/* Partition calculated Share of              */
  long int inf1psxia;		/* Partition calculated Share of              */
  int inf1psxcc;		/* Count of intervals aggregated in           */
  int inf1psxic;		/* Count of intervals aggregated in           */
  long int inf1pucpa;		/* Partition Calculated Utilization           */
  long int inf1puifa;		/* Partition Calculated Utilization           */
  int inf1pucpc;		/* Count of intervals aggregated in           */
  int inf1puifc;		/* Count of intervals aggregated in           */
  long int inf1pguca;		/* LPAR Group Utilization                     */
  long int inf1pguia;		/* LPAR Group Utilization                     */
  int inf1pgucc;		/* Count of intervals aggregated in           */
  int inf1pguic;		/* Count of intervals aggregated in           */
  long int inf1putod;		/* Host TOD value at the time when            */
  long int inf1pactc;		/* Aggregate number of microseconds           */
  long int inf1plptc;		/* Aggregate number of microseconds           */
  long int inf1poltc;		/* Aggregate number of microseconds           */
  long int inf1pwttc;		/* Aggregate number of microseconds           */
  long int inf1pmtic;		/* Aggregate number of microseconds           */
  long int inf1pacti;		/* Aggregate number of microseconds           */
  long int inf1plpti;		/* Aggregate number of microseconds           */
  long int inf1polti;		/* Aggregate number of microseconds           */
  long int inf1pwtti;		/* Aggregate number of microseconds           */
  long int inf1pmtii;		/* Aggregate number of microseconds           */
};
/* Values for field "inf1pfl1" */
#define inf1pmte   0x80		/* Multithreading (MT) is enabled    @VRLDWDN */
#define inf1ppool  0x40		/* Reserved for IBM use.             @VRLDWDN */
#define inf1pwtfl  0x20		/* Wait Completion flag is set. This          */
/* Values for field "inf1pvl1" */
#define inf1pprc   0x80		/* Processor Configuration Validity.          */
#define inf1pwcv   0x40		/* Partition weight-based capped              */
#define inf1pacc   0x20		/* Partition absolute capped capacity         */
#define inf1pidv   0x10		/* Partition ID Validity.                     */
#define inf1plgv   0x08		/* LPAR Group Absolute Capacity               */
#define inf1pplnv  0x04		/* Reserved for IBM use.             @VRLDWDN */
#define inf1penv   0x02		/* Partition Entitlement and                  */
#define inf1putv   0x01		/* Partition Core Utilization and             */
/* Values for field "inf1pcmod" */
#define inf1pcmgn  0x80		/* General (ESA390) logical                   */
#define inf1pcmli  0x40		/* Linux logical configuration mode @VRLDWDN  */
#define inf1pcmvm  0x20		/* VM logical configuration mode    @VRLDWDN  */
#define inf1pcmcf  0x10		/* CF logical configuration mode    @VRLDWDN  */
/* Values for field "inf1prcpu" */
#define inf1puccp  0x00		/* General Purpose (CP)             @VRLDWDN  */
#define inf1pucifl 0x03		/* Integrated Fac for Linux (IFL).  @VRLDWDN  */
/* Values for field "inf1pmtii" */
#define inf1psb1   0xE8		/* Version 1 length in bytes                  */
#define inf1psd1   0x1D		/* Version 1 length in doublewords            */
struct inf1hyp
{
  unsigned char inf1yfl1;	/* Hypervisor Flag Byte 1            @VRLDWDN */
  unsigned char inf1yfl2;	/* Hypervisor Flag Byte 2                     */
  unsigned char inf1yvl1;	/* Reserved for IBM use              @VRLDWDN */
  unsigned char inf1yvl2;	/* Reserved for IBM use              @VRLDWDN */
  char inf1ytyp;		/* Hypervisor type                   @VRLDWDN */
  unsigned char _filler1;	/* Reserved for IBM use              @VRLDWDN */
  unsigned char inf1ycpt;	/* Threads in use per CP core.                */
  unsigned char inf1yift;	/* Threads in use per IFL core.               */
  unsigned char inf1ysid[8];	/* System Identifier, in EBCDIC               */
  unsigned char inf1ycln[8];	/* Cluster Name, in EBCDIC format,            */
  short int inf1yscp;		/* Number of CP cores shared by               */
  short int inf1ydcp;		/* Number of CP cores dedicated               */
  short int inf1ysif;		/* Number of IFL cores shared by              */
  short int inf1ydif;		/* Number of IFL cores dedicated              */
  int inf1yasc;			/* Sum of absolute shares for all             */
  int inf1yasi;			/* Sum of absolute shares for all             */
  int inf1yrsc;			/* Sum of relative shares for all             */
  int inf1yrsi;			/* Sum of relative shares for all             */
  int inf1ylcc;			/* Approximate count of adds to               */
  int inf1ylci;			/* Approximate count of adds to               */
  int inf1ymonh;		/* Current Monitor high-frequency (HF)        */
  char inf1ypkf;		/* HiperDispatch Unparking setting.           */
  char inf1yxusc;		/* SRM EXCESSUSE for General (CP)             */
  char inf1yxusi;		/* SRM EXCESSUSE for IFL processor            */
  unsigned char _filler2;	/* Reserved for IBM use              @VRLDWDN */
  int inf1ypadc;		/* SRM CPUPAD value, a scaled number          */
  int inf1ypadi;		/* SRM CPUPAD value, a scaled number          */
  long int inf1yutod;		/* TOD value at the start of the              */
  long int inf1yutc;		/* Total CPU time in microseconds             */
  long int inf1yuti;		/* Total CPU time in microseconds             */
  long int inf1ystc;		/* Total CPU time in microseconds             */
  long int inf1ysti;		/* Total CPU time in microseconds             */
  long int inf1ywtc;		/* Total elapsed time in microseconds         */
  long int inf1ywti;		/* Total elapsed time in microseconds         */
  long int inf1yptc;		/* Total elapsed time in microseconds         */
  long int inf1ypti;		/* Total elapsed time in microseconds         */
};
/* Values for field "inf1yfl1" */
#define inf1ylmc 0x80		/* Consumption method is used to              */
#define inf1ylmp 0x40		/* If on, Limithard caps use                  */
#define inf1ymte 0x20		/* Hypervisor is MT-enabled.         @VRLDWDN */
#define inf1yvrt 0x10		/* Vertical polarization is in use.           */
/* Values for field "inf1ytyp" */
#define inf1ytvm 0x01		/* z/VM is the hypervisor            @VRLDWDN */
/* Values for field "inf1ypkf" */
#define inf1yplg 0x00		/* SRM UNPARKING LARGE (default)     @VRLDWDN */
#define inf1ypmd 0x01		/* SRM UNPARKING MEDIUM              @VRLDWDN */
#define inf1ypsm 0x02		/* SRM UNPARKING SMALL               @VRLDWDN */
/* Values for field "inf1yxusc" */
#define inf1yxhi 16		/* -Indicates the system should be            */
#define inf1yxmd 8		/* -Indicates the system should be            */
#define inf1yxlo 1		/* -Indicates the system should not           */
#define inf1yxno 2		/* -Indicates the system should not           */
/* Values for field "inf1ypti" */
#define inf1ysb1 0x90		/* Version 1 length in bytes                  */
#define inf1ysd1 0x12		/* Version 1 length in doublewords            */
struct inf1gst
{
  unsigned char inf1gsti[320];	/* Mapped by INF3GST which lacks the          */
};
/* Values for field "inf1gsti" */
#define inf1gsb1 0x140		/* Version 1 length in bytes                */
#define inf1gsd1 0x28		/* Version 1 length in doublewords          */
struct inf2bk
{
  union
  {
    unsigned char inf2chdr[64];	/* Common section of header mapped            */
    struct
    {
      short int inf2vrsn;	/* Response buffer version.        @UB040DN */
      unsigned char _filler1[62];
    };
  };
};
/* Values for field "inf2vrsn" */
#define inf2v00001 0x01		/* Initial version. APAR VM66105.    @VRLDWDN */
#define inf2bsb1   0x40		/* Version 1 length in bytes                  */
#define inf2bsd1   0x08		/* Version 1 length in doublewords            */
struct inf2gst
{
  unsigned char inf2guid[8];	/* Guest's userid, in EBCDIC format. @VRLDWDN */
  unsigned char inf2gacn[8];	/* User accounting number in EBCDIC           */
  int inf2gtod;			/* Bits 0-31 of host TOD at guest             */
  unsigned char inf2gflg;	/* Guest list entry flag byte 1.     @VRLDWDN */
  char inf2gcmod;		/* Virtual Configuration Mode        @VRLDWDN */
  unsigned char inf2gaffn;	/* Guest CPUAFFINITY settings.       @VRLDWDN */
  unsigned char inf2gprtp;	/* Guest primary virtual CPU type.   @VRLDWDN */
  unsigned char inf2gprdt;	/* Guest primary vCPU dispatch type. @VRLDWDN */
  unsigned char _filler1[7];	/* Reserved for IBM use.             @VRLDWDN */
};
/* Values for field "inf2gflg" */
#define inf2glini  0x08		/* Guest identified itself as running         */
#define inf2glinh  0x04		/* Guest may be running Linux based           */
/* Values for field "inf2gcmod" */
#define inf2gcmgn  0x80		/* General (ESA390) virtual                   */
#define inf2gcmli  0x40		/* Linux virtual configuration mode @VRLDWDN  */
#define inf2gcmvm  0x20		/* VM virtual configuration mode    @VRLDWDN  */
#define inf2gcmcf  0x10		/* CF virtual configuration mode    @VRLDWDN  */
/* Values for field "inf2gaffn" */
#define inf2gafon  0x80		/* CPUAFFINITY is ON, but may be              */
#define inf2gafsup 0x40		/* CPUAFFINTY is suppressed.        @VRLDWDN  */
/* Values for field "inf2gprtp" */
#define inf2puccp  0x00		/* General Purpose (CP)             @VRLDWDN  */
#define inf2pucifl 0x03		/* Integrated Fac for Linux (IFL).  @VRLDWDN  */
/* Values for field "_filler1" */
#define inf2gsb1   0x20		/* Version 1 length in bytes                  */
#define inf2gsd1   0x04		/* Version 1 length in doublewords            */
struct inf3bk
{
  union
  {
    unsigned char inf3chdr[64];	/* Common section of header mapped            */
    struct
    {
      short int inf3vrsn;	/* Response buffer version.        @UB040DN */
      unsigned char _filler1[62];
    };
  };
  unsigned char inf3gsti[320];	/* Guest Information section                  */
};
/* Values for field "inf3vrsn" */
#define inf3v00001 0x01		/* Initial version. APAR VM66105.    @VRLDWDN */
/* Values for field "inf3gsti" */
#define inf3bsb1   0x180	/* Version 1 length in bytes                  */
#define inf3bsd1   0x30		/* Version 1 length in doublewords            */
struct inf3gst
{
  unsigned char inf3guid[8];	/* Guest's userid, in EBCDIC format. @VRLDWDN */
  unsigned char inf3gacn[8];	/* User accounting number in EBCDIC           */
  unsigned char inf3gflg;	/* Guest Flag Byte                   @VRLDWDN */
  unsigned char inf3gval;	/* Reserved for IBM use              @VRLDWDN */
  char inf3gcmod;		/* Virtual Configuration Mode        @VRLDWDN */
  unsigned char inf3gprtp;	/* Guest primary virtual CPU type.   @VRLDWDN */
  int inf3gtod;			/* Bits 0-31 of host TOD at guest             */
  unsigned char inf3gpna[8];	/* Resource pool name. Blanks if not          */
  int inf3giwsc;		/* I/O Wait Samples for CPs.                  */
  int inf3gcfsc;		/* Console Function Wait samples              */
  int inf3gsmsc;		/* Simulation Wait samples for CPs.           */
  int inf3gpwsc;		/* Page Wait Samples for CPs.                 */
  int inf3glsc;			/* Limit list Samples for CPs.                */
  int inf3gdsc;			/* CPU Delay Samples for CPs.                 */
  int inf3gcsc;			/* CPU Using Samples for CPs.                 */
  int inf3gessc;		/* E-list SVM Wait Samples for CPs.           */
  int inf3gldsc;		/* Loading User Samples for CPs.              */
  int inf3gdlsc;		/* Dormant User Samples for CPs.              */
  int inf3gdssc;		/* SVM Wait Samples for CPs.                  */
  int inf3giasc;		/* I/O Active Samples for CPs.                */
  int inf3gtisc;		/* Test Idle Samples for CPs.                 */
  int inf3gtssc;		/* Test Idle & SVM Wait Samples for           */
  int inf3gpasc;		/* Page Fault Active Samples for              */
  int inf3gosc;			/* Other Samples for CPs.                     */
  int inf3gtsc;			/* Total Samples for CPs.                     */
  int inf3giwsi;		/* I/O Wait Samples for IFLs.                 */
  int inf3gcfsi;		/* Console Function Wait samples              */
  int inf3gsmsi;		/* Simulation Wait samples for IFLs.          */
  int inf3gpwsi;		/* Page Wait Samples for IFLs.                */
  int inf3glsi;			/* Limit list Samples for IFLs.               */
  int inf3gdsi;			/* CPU Delay Samples for IFLs.                */
  int inf3gcsi;			/* CPU Using Samples for IFLs.                */
  int inf3gessi;		/* E-list SVM Wait Samples for IFLs.          */
  int inf3gldsi;		/* Loading User Samples for IFLs.             */
  int inf3gdlsi;		/* Dormant User Samples for IFLs.             */
  int inf3gdssi;		/* SVM Wait Samples for IFLs.                 */
  int inf3giasi;		/* I/O Active Samples for IFLs.               */
  int inf3gtisi;		/* Test Idle Samples for IFLs.                */
  int inf3gtssi;		/* Test Idle & SVM Wait Samples for           */
  int inf3gpasi;		/* Page Fault Active Samples for              */
  int inf3gosi;			/* Other Samples for IFLs.                    */
  int inf3gtsi;			/* Total Samples for IFLs.                    */
  unsigned char inf3cflg;	/* Guest CPU Flag Byte               @VRLDWDN */
  unsigned char inf3caffn;	/* Guest CPUAFFINITY settings.       @VRLDWDN */
  short int inf3cmcpu;		/* Maximum number of guest CPUs               */
  int _filler1;			/* Reserved for IBM use.             @VRLDWDN */
  long int inf3ctcpp;		/* Total virtual and simulation time          */
  long int inf3ctcps;		/* Total virtual and simulation time          */
  long int inf3ctcrp;		/* Total virtual and simulation time          */
  long int inf3ctcrs;		/* Total virtual and simulation time          */
  short int inf3cscp;		/* Number of guest shared CPs.       @VRLDWDN */
  short int inf3cdcp;		/* Number of guest dedicated CPs.    @VRLDWDN */
  short int inf3crcp;		/* Number of non-stopped guest CPs.  @VRLDWDN */
  short int _filler2;		/* Reserved for IBM use.             @VRLDWDN */
  unsigned char inf3ccdt;	/* Dispatch type for guest CPs                */
  unsigned char inf3ccscf;	/* Current share flags for CPs.      @VRLDWDN */
  unsigned char inf3ciscf;	/* Initial share flags for CPs.      @VRLDWDN */
  unsigned char _filler3;	/* Reserved for IBM use              @VRLDWDN */
  int inf3ccnsc;		/* Current normal relative share              */
  int inf3ccasc;		/* Current ABSOLUTE share for                 */
  int inf3ccmsc;		/* Current Max share for CP-dispatched        */
  int inf3cinsc;		/* Initial (logon) normal relative            */
  int inf3ciasc;		/* Initial (logon) ABSOLUTE share             */
  int inf3cimsc;		/* Initial (logon) Max share for              */
  int _filler4;			/* Reserved for IBM use.             @VRLDWDN */
  long int inf3ctipp;		/* Total virtual and simulation time          */
  long int inf3ctips;		/* Total virtual and simulation time          */
  long int inf3ctirp;		/* Total virtual and simulation time          */
  long int inf3ctirs;		/* Total virtual and simulation time          */
  short int inf3csif;		/* Number of guest shared IFLs.      @VRLDWDN */
  short int inf3cdif;		/* Number of guest dedicated IFLs.   @VRLDWDN */
  short int inf3crif;		/* Number of non-stopped guest IFLs. @VRLDWDN */
  short int _filler5;		/* Reserved for IBM use.             @VRLDWDN */
  unsigned char inf3cidt;	/* Dispatch type for guest IFLs.              */
  unsigned char inf3ccsif;	/* Current share flags for IFLs.     @VRLDWDN */
  unsigned char inf3cisif;	/* Initial share flags for IFLs.     @VRLDWDN */
  unsigned char _filler6;	/* Reserved for IBM use              @VRLDWDN */
  int inf3ccnsi;		/* Current normal relative share for          */
  int inf3ccasi;		/* Current ABSOLUTE share for                 */
  int inf3ccmsi;		/* Current Max share for IFL-dispatched       */
  int inf3cinsi;		/* Initial (logon) normal relative            */
  int inf3ciasi;		/* Initial (logon) ABSOLUTE share             */
  int inf3cimsi;		/* Initial (logon) Max share for              */
  int _filler7;			/* Reserved for IBM use.             @VRLDWDN */
};
/* Values for field "inf3gflg" */
#define inf3gmob   0x80		/* Guest mobility is enabled.        @VRLDWDN */
#define inf3glini  0x08		/* Guest identified itself as running         */
#define inf3glinh  0x04		/* Guest may be running Linux based           */
/* Values for field "inf3gcmod" */
#define inf3gcmgn  0x80		/* General (ESA390) virtual                   */
#define inf3gcmli  0x40		/* Linux virtual configuration mode @VRLDWDN  */
#define inf3gcmvm  0x20		/* VM virtual configuration mode    @VRLDWDN  */
#define inf3gcmcf  0x10		/* CF virtual configuration mode    @VRLDWDN  */
/* Values for field "inf3gprtp" */
#define inf3puccp  0x00		/* General Purpose (CP)             @VRLDWDN  */
#define inf3pucifl 0x03		/* Integrated Fac for Linux (IFL).  @VRLDWDN  */
/* Values for field "inf3cflg" */
#define inf3cmct   0x40		/* Guest has multiple CPU types      @VRLDWDN */
#define inf3cvct   0x20		/* Virtual CPs are thread dispatched @VRLDWDN */
#define inf3cvit   0x10		/* Virtual IFLs are thread dispatched@VRLDWDN */
/* Values for field "inf3caffn" */
#define inf3cafon  0x80		/* CPUAFFINITY is ON, but may be              */
#define inf3cafsup 0x40		/* CPUAFFINTY is suppressed.        @VRLDWDN  */
/* Values for field "inf3ccscf" */
#define inf3cclhc  0x40		/* Current max share for CP-dispatched        */
#define inf3ccnac  0x20		/* Current normal share for CP-dispatched     */
#define inf3ccmac  0x10		/* Current max share for CP-dispatched        */
/* Values for field "inf3ciscf" */
#define inf3cilhc  0x40		/* Initial max share for CP-dispatched        */
#define inf3cinac  0x20		/* Initial normal share for CP-dispatched     */
#define inf3cimac  0x10		/* Initial max share for CP-dispatched        */
/* Values for field "inf3ccsif" */
#define inf3cclhi  0x40		/* Current max share for IFL-dispatched       */
#define inf3ccnai  0x20		/* Current normal share for IFL-dispatched    */
#define inf3ccmai  0x10		/* Current max share for IFL-dispatched       */
/* Values for field "inf3cisif" */
#define inf3cilhi  0x40		/* Initial max share for IFL-dispatched       */
#define inf3cinai  0x20		/* Initial normal share for IFL-dispatched    */
#define inf3cimai  0x10		/* Initial max share for IFL-dispatched       */
/* Values for field "_filler7" */
#define inf3gsb1   0x140	/* Version 1 length in bytes                  */
#define inf3gsd1   0x28		/* Version 1 length in doublewords            */
struct inf4bk
{
  union
  {
    unsigned char inf4chdr[64];	/* Common section of header mapped            */
    struct
    {
      short int inf4vrsn;	/* Response buffer version.        @UB040DN */
      unsigned char _filler1[62];
    };
  };
};
/* Values for field "inf4vrsn" */
#define inf4v00001 0x01		/* Initial version. APAR VM66105.    @VRLDWDN */
#define inf4bsb1   0x40		/* Version 1 length in bytes                  */
#define inf4bsd1   0x08		/* Version 1 length in doublewords            */
struct inf4pool
{
  unsigned char inf4pnam[8];	/* Resource Pool name.               @VRLDWDN */
  unsigned char inf4pcre[8];	/* Resource Pool creator userid.     @VRLDWDN */
};
/* Values for field "inf4pcre" */
#define inf4psb1 0x10		/* Version 1 length in bytes               */
#define inf4psd1 0x02		/* Version 1 length in doublewords         */
struct inf5bk
{
  union
  {
    unsigned char inf5chdr[64];	/* Common section of header mapped            */
    struct
    {
      short int inf5vrsn;	/* Response buffer version.        @UB040DN */
      unsigned char _filler1[62];
    };
  };
  unsigned char inf5plna[8];	/* Resource pool name.               @VRLDWDN */
  unsigned char inf5pcre[8];	/* Resource Pool creator's user ID.  @VRLDWDN */
  long int inf5stmp;		/* Host TOD value at the time of the          */
  unsigned char inf5cflg;	/* Resource Pool CPU Capping Flags   @VRLDWDN */
  unsigned char _filler2[3];	/* Reserved for IBM use.             @VRLDWDN */
  int inf5clim;			/* Resource pool maximum share for            */
  long int inf5ctim;		/* Total Time consumed by guests vCPUs        */
  int inf5clmn;			/* Total number of times the Resource         */
  int inf5cctl;			/* Total number of times vCPUs of             */
  long int inf5ctml;		/* Total time limited for vCPUs of all        */
  int inf5cifla;		/* IFL CPUAFFINITY toggle sequence            */
  int _filler3;			/* Reserved for IBM use.             @VRLDWDN */
};
/* Values for field "inf5vrsn" */
#define inf5v00001 0x01		/* Initial version. APAR VM66105.    @VRLDWDN */
/* Values for field "inf5cflg" */
#define inf5cclc   0x80		/* Resource Pool's CP virtual type            */
#define inf5cccc   0x40		/* Resource Pool's CP virtual type            */
#define inf5cilc   0x20		/* Resource Pool's IFL virtual type           */
#define inf5cicc   0x10		/* Resource Pool's IFL virtual type           */
#define inf5cpct   0x08		/* Resource Pool uses prorated core           */
#define inf5csup   0x04		/* IFL CPUAFFINITY is suppressed.             */
/* Values for field "_filler3" */
#define inf5bsb1   0x80		/* Version 1 length in bytes                  */
#define inf5bsd1   0x10		/* Version 1 length in doublewords            */
struct inf6bk
{
  union
  {
    unsigned char inf6chdr[64];	/* Common section of header mapped            */
    struct
    {
      short int inf6vrsn;	/* Response buffer version.        @UB040DN */
      unsigned char _filler1[62];
    };
  };
  unsigned char inf6plna[8];	/* Resource pool name.               @VRLDWDN */
};
/* Values for field "inf6vrsn" */
#define inf6v00001 0x01		/* Initial version. APAR VM66105.    @VRLDWDN */
/* Values for field "inf6plna" */
#define inf6bsb1   0x48		/* Version 1 length in bytes                  */
#define inf6bsd1   0x09		/* Version 1 length in doublewords            */
struct inf6mem
{
  unsigned char inf6mnam[8];	/* Resource Pool member name.        @VRLDWDN */
};
/* Values for field "inf6mnam" */
#define inf6msb1 0x08		/* Version 1 length in bytes                */
#define inf6msd1 0x01		/* Version 1 length in doublewords          */
#pragma pack(reset)
