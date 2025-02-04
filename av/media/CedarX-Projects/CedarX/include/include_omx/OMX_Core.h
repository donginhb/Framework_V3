/*
 * Copyright (c) 2005 The Khronos Group Inc. 
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject
 * to the following conditions: 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software. 
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. 
 *
 */

/** OMX_Core.h
 *  The OMX_Core header file contains the definitions used by both the
 *  application and the component to access common items.
 */




#ifndef OMX_Core_h
#define OMX_Core_h

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/* Each OMX header shall include all required header files to allow the
 *  header to compile without errors.  The includes below are required
 *  for this header file to compile successfully 
 */
#include <include_omx/OMX_Types.h>
#include <include_omx/OMX_Index.h>
//#include <CDX_Common.h>

/** The OMX_COMMANDTYPE enumeration is used to specify the action in the
 *  OMX_SendCommand macro.  
 */
typedef enum OMX_COMMANDTYPE
{
    OMX_CommandStateSet,    /**< Change the component state */
    OMX_CommandFlush,       /**< Flush the data queue(s) of a component */
    OMX_CommandPortDisable, /**< Disable a port on a component. */
    OMX_CommandPortEnable,  /**< Enable a port on a component. */
    OMX_CommandMarkBuffer,  /**< Mark a component/buffer for observation */
    OMX_CommandKhronosExtensions = 0x6F000000, /**< Reserved region for introducing Khronos Standard Extensions */ 
                                                /**< Reserved region for introducing Vendor Extensions */
    OMX_CommandVendorAddOutputSinkInfo = 0x7F000000,
    OMX_CommandVendorRemoveOutputSinkInfo,
	OMX_CommandSwitchFile,  //immediately switch file

    OMX_CommandMax = 0X7FFFFFFF
} OMX_COMMANDTYPE;



/** The OMX_STATETYPE enumeration is used to indicate or change the component
 *  state.  This enumeration reflects the current state of the component when
 *  used with the OMX_GetState macro or becomes the parameter in a state change
 *  command when used with the OMX_SendCommand macro.
 *
 *  The component will be in the Loaded state after the component is initially
 *  loaded into memory.  In the Loaded state, the component is not allowed to
 *  allocate or hold resources other than to build it's internal parameter
 *  and configuration tables.  The application will send one or more
 *  SetParameters/GetParameters and SetConfig/GetConfig commands to the
 *  component and the component will record each of these parameter and
 *  configuration changes for use later.  When the application sends the
 *  Idle command, the component will acquire the resources needed for the
 *  specified configuration and will transition to the idle state if the
 *  allocation is successful.  If the component cannot successfully
 *  transition to the idle state for any reason, the state of the component
 *  shall be fully rolled back to the Loaded state (e.g. all allocated 
 *  resources shall be released).  When the component receives the command
 *  to go to the Executing state, it shall begin processing buffers by
 *  sending all input buffers it holds to the application.  While
 *  the component is in the Idle state, the application may also send the
 *  Pause command.  If the component receives the pause command while in the
 *  Idle state, the component shall send all input buffers it holds to the 
 *  application, but shall not begin processing buffers.  This will allow the
 *  application to prefill buffers.
 */

typedef enum OMX_STATETYPE
{
    OMX_StateInvalid,      /**< component has detected that it's internal data 
                                structures are corrupted to the point that
                                it cannot determine it's state properly */
    OMX_StateLoaded,      /**< component has been loaded but has not completed
                                initialization.  The OMX_SetParameter macro
                                and the OMX_GetParameter macro are the only 
                                valid macros allowed to be sent to the 
                                component in this state. */
    OMX_StateIdle,        /**< component initialization has been completed
                                successfully and the component is ready to
                                to start. */
    OMX_StateExecuting,   /**< component has accepted the start command and
                                is processing data (if data is available) */
    OMX_StatePause,       /**< component has received pause command */
    OMX_StateWaitForResources, /**< component is waiting for resources, either after 
                                preemption or before it gets the resources requested.
                                See specification for complete details. */

    //OMX_StatePreparing,     /*vendor defined state for network stream*/

    OMX_StateMax = 0X7FFFFFFF
} OMX_STATETYPE;

/** The OMX_ERRORTYPE enumeration defines the standard OMX Errors.  These 
 *  errors should cover most of the common failure cases.  However, 
 *  vendors are free to add additional error messages of their own as 
 *  long as they follow these rules:
 *  1.  Vendor error messages shall be in the range of 0x90000000 to
 *      0x9000FFFF.
 *  2.  Vendor error messages shall be defined in a header file provided
 *      with the component.  No error messages are allowed that are
 *      not defined.
 */
typedef enum OMX_ERRORTYPE
{
  OMX_ErrorNone = 0,

  /** There were insufficient resources to perform the requested operation */
  OMX_ErrorInsufficientResources = 0x80001000,

  /** There was an error, but the cause of the error could not be determined */
  OMX_ErrorUndefined = 0x80001001,

  /** The component name string was not valid */
  OMX_ErrorInvalidComponentName = 0x80001002,

  /** No component with the specified name string was found */
  OMX_ErrorComponentNotFound = 0x80001003,

  /** The component specified did not have a "OMX_ComponentInit" or
      "OMX_ComponentDeInit entry point */
  OMX_ErrorInvalidComponent = 0x80001004,

  /** One or more parameters were not valid */
  OMX_ErrorBadParameter = 0x80001005,

  /** The requested function is not implemented */
  OMX_ErrorNotImplemented = 0x80001006,

  /** The buffer was emptied before the next buffer was ready */
  OMX_ErrorUnderflow = 0x80001007,

  /** The buffer was not available when it was needed */
  OMX_ErrorOverflow = 0x80001008,

  /** The hardware failed to respond as expected */
  OMX_ErrorHardware = 0x80001009,

  /** The component is in the state OMX_StateInvalid */
  OMX_ErrorInvalidState = 0x8000100A,

  /** Stream is found to be corrupt */
  OMX_ErrorStreamCorrupt = 0x8000100B,

  /** Ports being connected are not compatible */
  OMX_ErrorPortsNotCompatible = 0x8000100C,

  /** Resources allocated to an idle component have been
      lost resulting in the component returning to the loaded state */
  OMX_ErrorResourcesLost = 0x8000100D,

  /** No more indicies can be enumerated */
  OMX_ErrorNoMore = 0x8000100E,

  /** The component detected a version mismatch */
  OMX_ErrorVersionMismatch = 0x8000100F,

  /** The component is not ready to return data at this time */
  OMX_ErrorNotReady = 0x80001010,

  /** There was a timeout that occurred */
  OMX_ErrorTimeout = 0x80001011,

  /** This error occurs when trying to transition into the state you are already in */
  OMX_ErrorSameState = 0x80001012,

  /** Resources allocated to an executing or paused component have been 
      preempted, causing the component to return to the idle state */
  OMX_ErrorResourcesPreempted = 0x80001013, 

  /** A non-supplier port sends this error to the IL client (via the EventHandler callback) 
      during the allocation of buffers (on a transition from the LOADED to the IDLE state or
      on a port restart) when it deems that it has waited an unusually long time for the supplier 
      to send it an allocated buffer via a UseBuffer call. */
  OMX_ErrorPortUnresponsiveDuringAllocation = 0x80001014,

  /** A non-supplier port sends this error to the IL client (via the EventHandler callback) 
      during the deallocation of buffers (on a transition from the IDLE to LOADED state or 
      on a port stop) when it deems that it has waited an unusually long time for the supplier 
      to request the deallocation of a buffer header via a FreeBuffer call. */
  OMX_ErrorPortUnresponsiveDuringDeallocation = 0x80001015,

  /** A supplier port sends this error to the IL client (via the EventHandler callback) 
      during the stopping of a port (either on a transition from the IDLE to LOADED 
      state or a port stop) when it deems that it has waited an unusually long time for 
      the non-supplier to return a buffer via an EmptyThisBuffer or FillThisBuffer call. */
  OMX_ErrorPortUnresponsiveDuringStop = 0x80001016,

  /** Attempting a state transtion that is not allowed */
  OMX_ErrorIncorrectStateTransition = 0x80001017,

  /* Attempting a command that is not allowed during the present state. */
  OMX_ErrorIncorrectStateOperation = 0x80001018, 

  /** The values encapsulated in the parameter or config structure are not supported. */
  OMX_ErrorUnsupportedSetting = 0x80001019,

  /** The parameter or config indicated by the given index is not supported. */
  OMX_ErrorUnsupportedIndex = 0x8000101A,

  /** The port index supplied is incorrect. */
  OMX_ErrorBadPortIndex = 0x8000101B,

  /** The port has lost one or more of its buffers and it thus unpopulated. */
  OMX_ErrorPortUnpopulated = 0x8000101C,

  OMX_ErrorMax = 0x7FFFFFFF,
} OMX_ERRORTYPE;

typedef OMX_ERRORTYPE (* OMX_COMPONENTINITTYPE)(OMX_IN  OMX_HANDLETYPE hComponent);

typedef struct OMX_COMPONENTREGISTERTYPE
{
  const char          * pName;       /* Component name, 128 byte limit (including 锟絓0锟� applies */
  OMX_COMPONENTINITTYPE pInitialize; /* Component instance initialization function */
} OMX_COMPONENTREGISTERTYPE;

extern OMX_COMPONENTREGISTERTYPE OMX_ComponentRegistered[];
 

/** The OMX_VERSIONTYPE union is used to specify the version for
    a structure or component.  For a component, the version is entirely
    specified by the component vendor.  Components doing the same function
    from different vendors may or may not have the same version.  For 
    structures, the version shall be set by the entity that allocates the
    structure.  For structures specified in the OMX 1.0 specification, the
    value of the version shall be set to 1.0.0.0 in all cases.  Access to the
    OMX_VERSIONTYPE can be by a single 32 bit access (e.g. by nVersion) or
    by accessing one of the structure elements to, for example, check only
    the Major revision.
 */
typedef union OMX_VERSIONTYPE
{
    struct
    {
        OMX_U8 nVersionMajor;   /**< Major version accessor element */
        OMX_U8 nVersionMinor;   /**< Minor version accessor element */
        OMX_U8 nRevision;       /**< Revision version accessor element */
        OMX_U8 nStep;           /**< Step version accessor element */
    } s;
    OMX_U32 nVersion;           /**< 32 bit value to make accessing the
                                    version easily done in a single word
                                    size copy/compare operation */
} OMX_VERSIONTYPE;

typedef struct OMX_PRIORITYMGMTTYPE {
 OMX_U32 nSize;             /**< size of the structure in bytes */
 OMX_VERSIONTYPE nVersion;  /**< OMX specification version information */
 OMX_U32 nGroupPriority;            /**< Priority of the component group */
 OMX_U32 nGroupID;                  /**< ID of the component group */
} OMX_PRIORITYMGMTTYPE;

/** End of Stream Buffer Flag: 
  *
  * A component sets EOS when it has no more data to emit on a particular 
  * output port. Thus an output port shall set EOS on the last buffer it 
  * emits. A component's determination of when an output port should 
  * cease sending data is implemenation specific.
  */

#define OMX_BUFFERFLAG_EOS 0x00000001 

/** Start Time Buffer Flag: 
 *
 * The source of a stream (e.g. a demux component) sets the STARTTIME
 * flag on the buffer that contains the starting timestamp for the
 * stream. The starting timestamp corresponds to the first data that
 * should be displayed at startup or after a seek.
 * The first timestamp of the stream is not necessarily the start time.
 * For instance, in the case of a seek to a particular video frame, 
 * the target frame may be an interframe. Thus the first buffer of 
 * the stream will be the intra-frame preceding the target frame and
 * the starttime will occur with the target frame (with any other
 * required frames required to reconstruct the target intervening).
 *
 * The STARTTIME flag is directly associated with the buffer锟絪 
 * timestamp 锟�thus its association to buffer data and its 
 * propagation is identical to the timestamp锟絪.
 *
 * When a Sync Component client receives a buffer with the 
 * STARTTIME flag it shall perform a SetConfig on its sync port 
 * using OMX_ConfigTimeClientStartTime and passing the buffer锟絪
 * timestamp.
 */

#define OMX_BUFFERFLAG_STARTTIME 0x00000002

 

/** Decode Only Buffer Flag: 
 *
 * The source of a stream (e.g. a demux component) sets the DECODEONLY
 * flag on any buffer that should shall be decoded but should not be
 * displayed. This flag is used, for instance, when a source seeks to 
 * a target interframe that requires the decode of frames preceding the 
 * target to facilitate the target锟絪 reconstruction. In this case the 
 * source would emit the frames preceding the target downstream 
 * but mark them as decode only.
 *
 * The DECODEONLY is associated with buffer data and propagated in a 
 * manner identical to the buffer timestamp.
 *
 * A component that renders data should ignore all buffers with 
 * the DECODEONLY flag set.
 */

#define OMX_BUFFERFLAG_DECODEONLY 0x00000004


/* Data Corrupt Flag: This flag is set when the IL client believes the data in the associated buffer is corrupt */

#define OMX_BUFFERFLAG_DATACORRUPT 0x00000008

/* End of Frame: The buffer contains exactly one end of frame and no data
   occurs after the end of frame. This flag is an optional hint. The absence
   of this flag does not imply the absence of an end of frame within the buffer. */

#define OMX_BUFFERFLAG_ENDOFFRAME 0x00000010

#define OMX_NOPORT 0xFFFFFFFE

typedef struct OMX_BUFFERHEADERTYPE
{
    OMX_U32 nSize;              /**< size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U8* pBuffer;            /**< Pointer to actual block of memory 
                                     that is acting as the buffer */
    OMX_U8* pBufferExtra;       /**< Pointer to actual block of memory 
                                     that is acting as the buffer */
    OMX_U32 nBufferLen;         /**< size of the buffer , in bytes */
    OMX_U32 nBufferExtraLen;    /**< size of the buffer extra, in bytes */
	
    OMX_S32 nTobeFillLen;
    OMX_S32 nBufferID;
    OMX_U32 nCtrlBits;

    OMX_U32 nAllocLen;          /**< size of the buffer allocated, in bytes */
    OMX_U32 nFilledLen;         /**< number of bytes currently in the 
                                     buffer */
    OMX_U32 nOffset;            /**< start offset of valid data in bytes from
                                     the start of the buffer */
    OMX_PTR pAppPrivate;        /**< pointer to any data the application
                                     wants to associate with this buffer */
    OMX_PTR pPlatformPrivate;   /**< pointer to any data the platform
                                     wants to associate with this buffer */ 
    OMX_PTR pInputPortPrivate;  /**< pointer to any data the input port
                                     wants to associate with this buffer */
    OMX_PTR pOutputPortPrivate; /**< pointer to any data the output port
                                     wants to associate with this buffer */
    OMX_HANDLETYPE hMarkTargetComponent; /**< The component that will generate a 
                                              mark event upon processing this buffer. */
    OMX_PTR pMarkData;          /**< Application specific data associated with 
                                     the mark sent on a mark event to disambiguate 
                                     this mark from others. */
    OMX_U32 nTickCount;         /**< Optional entry that the component and
                                     application can update with a tick count
                                     when they access the component.  This
                                     value should be in microseconds.  Since
                                     this is a value relative to an arbitrary
                                     starting point, this value cannot be used 
                                     to determine absolute time.  This is an
                                     optional entry and not all components
                                     will update it.*/
  OMX_TICKS nTimeStamp;         /**< Timestamp corresponding to the sample
                                     starting at the first logical sample 
                                     boundary in the buffer. Timestamps of 
                                     successive samples within the buffer may
                                     be inferred by adding the duration of the 
                                     of the preceding buffer to the timestamp
                                     of the preceding buffer.*/
  OMX_S64  duration; 			/*  for subtitle */
  OMX_U32     nFlags;           /**< buffer specific flags */
  OMX_U32 nOutputPortIndex;     /**< The index of the output port (if any) using 
                                     this buffer */
  OMX_U32 nInputPortIndex;      /**< The index of the input port (if any) using
                                     this buffer */
                                        

  unsigned int video_stream_type;   //CDX_VIDEO_STREAM_MAJOR
  OMX_U32   subDispEndTime;
                                     
} OMX_BUFFERHEADERTYPE;

typedef struct OMX_BUFFERSTATE{
  OMX_U32 nValidSizePercent;
  OMX_U32 nElementCounter;
  unsigned int video_stream_type;   //CDX_VIDEO_STREAM_MAJOR
}OMX_BUFFERSTATE;

typedef struct OMX_PORT_PARAM_TYPE {
    OMX_U32 nSize;              /**< size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPorts;             /**< The number of ports for this component */
    OMX_U32 nStartPortNumber;   /** first port number for this type of port */
} OMX_PORT_PARAM_TYPE; 

typedef enum OMX_EVENTTYPE
{
    OMX_EventCmdComplete,         /**< component has sucessfully completed a command */
    OMX_EventError,               /**< component has detected an error condition */
    OMX_EventMark,                /**< component has detected a buffer mark */
    OMX_EventPortSettingsChanged, /**< component is reported a port settings change */
    OMX_EventBufferFlag,          /**< component has detected an EOS */ 
    OMX_EventResourcesAcquired,   /**< component has been granted resources and is
                                       automatically starting the state change from
                                       OMX_StateWaitForResources to OMX_StateIdle. */
    OMX_EventKeyFrameDecoded,
    OMX_EventBufferPrefilled,
    OMX_EventBufferMayAvaiable,

    OMX_EventControlPause,
    OMX_EventControlStart,
    OMX_EventBufferUpdate,
    OMX_EventBufferStart,
    OMX_EventBufferEnd,
    OMX_EventAVSyncException,
    OMX_EventWholeBufferUpdate,

    OMX_EventMoreBuffer,
    OMX_EventInfo,
	OMX_EventRawdata,
    OMX_EventRecordDone,
	OMX_EventMultiPixelExit,

	OMX_EventNeedNextFd,
	OMX_EventRecVbvFull,
	OMX_EventWriteDiskError,

    OMX_EventMax = 0x7FFFFFFF
} OMX_EVENTTYPE;

typedef struct OMX_CALLBACKTYPE
{
    /** The EventHandler method is used to notify the application when an
        event of interest occurs.  Events are defined in the OMX_EVENTTYPE
        enumeration.  Please see that enumeration for details of what will
        be returned for each type of event. Callbacks should not return
        an error to the component, so if an error occurs, the application 
        shall handle it internally.  This is a blocking call.

        The application should return from this call within 5 msec to avoid
        blocking the component for an excessively long period of time.

        @param hComponent
            handle of the component to access.  This is the component
            handle returned by the call to the GetHandle function.
        @param pAppData
            pointer to an application defined value that was provided in the 
            pAppData parameter to the OMX_GetHandle method for the component.
            This application defined value is provided so that the application 
            can have a component specific context when receiving the callback.
        @param eEvent
            Event that the component wants to notify the application about.
        @param nData1
            nData will be the OMX_ERRORTYPE for an error event and will be 
            an OMX_COMMANDTYPE for a command complete event and OMX_INDEXTYPE for a OMX_PortSettingsChanged event.
         @param nData2
            nData2 will hold further information related to the event. Can be OMX_STATETYPE for
            a OMX_StateChange command or port index for a OMX_PortSettingsCHanged event.
            Default value is 0 if not used. )
        @param pEventData
            Pointer to additional event-specific data (see spec for meaning).
      */

   OMX_ERRORTYPE (*EventHandler)(
        OMX_IN OMX_HANDLETYPE hComponent,
        OMX_IN OMX_PTR pAppData,
        OMX_IN OMX_EVENTTYPE eEvent,
        OMX_IN OMX_U32 nData1,
        OMX_IN OMX_U32 nData2,
        OMX_IN OMX_PTR pEventData);

    /** The EmptyBufferDone method is used to return emptied buffers from an
        input port back to the application for reuse.  This is a blocking call 
        so the application should not attempt to refill the buffers during this
        call, but should queue them and refill them in another thread.  There
        is no error return, so the application shall handle any errors generated
        internally.  
        
        The application should return from this call within 5 msec.
        
        @param hComponent
            handle of the component to access.  This is the component
            handle returned by the call to the GetHandle function.
        @param pAppData
            pointer to an application defined value that was provided in the 
            pAppData parameter to the OMX_GetHandle method for the component.
            This application defined value is provided so that the application 
            can have a component specific context when receiving the callback.
        @param pBuffer
            pointer to an OMX_BUFFERHEADERTYPE structure allocated with UseBuffer
            or AllocateBuffer indicating the buffer that was emptied.
     */
    OMX_ERRORTYPE (*EmptyBufferDone)(
        OMX_IN OMX_HANDLETYPE hComponent,
        OMX_IN OMX_PTR pAppData,
        OMX_IN OMX_BUFFERHEADERTYPE* pBuffer);

    /** The FillBufferDone method is used to return filled buffers from an
        output port back to the application for emptying and then reuse.  
        This is a blocking call so the application should not attempt to 
        empty the buffers during this call, but should queue the buffers 
        and empty them in another thread.  There is no error return, so 
        the application shall handle any errors generated internally.  The 
        application shall also update the buffer header to indicate the
        number of bytes placed into the buffer.  

        The application should return from this call within 5 msec.
        
        @param hComponent
            handle of the component to access.  This is the component
            handle returned by the call to the GetHandle function.
        @param pAppData
            pointer to an application defined value that was provided in the 
            pAppData parameter to the OMX_GetHandle method for the component.
            This application defined value is provided so that the application 
            can have a component specific context when receiving the callback.
        @param pBuffer
            pointer to an OMX_BUFFERHEADERTYPE structure allocated with UseBuffer
            or AllocateBuffer indicating the buffer that was filled.
     */
    OMX_ERRORTYPE (*FillBufferDone)(
        OMX_OUT OMX_HANDLETYPE hComponent,
        OMX_OUT OMX_PTR pAppData,
        OMX_OUT OMX_BUFFERHEADERTYPE* pBuffer);

} OMX_CALLBACKTYPE;

/** The OMX_BUFFERSUPPLIERTYPE enumeration is used to dictate port supplier
    preference when tunneling between two ports.
*/
typedef enum OMX_BUFFERSUPPLIERTYPE
{
    OMX_BufferSupplyUnspecified = 0x0, /**< port supplying the buffers is unspecified,
                                              or don't care */
    OMX_BufferSupplyInput,             /**< input port supplies the buffers */
    OMX_BufferSupplyOutput,            /**< output port supplies the buffers */
    OMX_BufferSupplyMax = 0x7FFFFFFF
} OMX_BUFFERSUPPLIERTYPE;


/** buffer supplier parameter */
typedef struct OMX_PARAM_BUFFERSUPPLIERTYPE {
    OMX_U32 nSize; /**< size of the structure in bytes */
    OMX_VERSIONTYPE nVersion; /**< OMX specification version information */
    OMX_U32 nPortIndex; /**< port that this structure applies to */
    OMX_BUFFERSUPPLIERTYPE eBufferSupplier; /**< buffer supplier */
} OMX_PARAM_BUFFERSUPPLIERTYPE;


/**< indicates that buffers received by an input port of a tunnel 
     may not modify the data in the buffers */
#define OMX_PORTTUNNELFLAG_READONLY 0x00000001 


/** The OMX_TUNNELSETUPTYPE structure is used to pass data from an output
    port to an input port as part the two ComponentTunnelRequest calls
    resulting from a OMX_SetupTunnel call from the IL Client. */   
typedef struct OMX_TUNNELSETUPTYPE
{
    OMX_U32 nTunnelFlags;             /**< bit flags for tunneling */
    OMX_BUFFERSUPPLIERTYPE eSupplier; /**< supplier preference */
} OMX_TUNNELSETUPTYPE; 

/* OMX Component headers is included to enable the core to use
   macros for functions into the component for OMX release 1.0.  
   Developers should not access any structures or data from within
   the component header directly */
//#include <OMX_Component.h>

/** GetComponentVersion will return information about the component.  
    This is a blocking call.  This macro will go directly from the
    application to the component (via a core macro).  The
    component will return from this call within 5 msec.
    @param [in] hComponent
        handle of component to execute the command
    @param [out] pComponentName
        pointer to an empty string of length 128 bytes.  The component 
        will write its name into this string.  The name will be 
        terminated by a single zero byte.  The name of a component will 
        be 127 bytes or less to leave room for the trailing zero byte.  
        An example of a valid component name is "OMX.ABC.ChannelMixer\0".
    @param [out] pComponentVersion
        pointer to an OMX Version structure that the component will fill 
        in.  The component will fill in a value that indicates the 
        component version.  NOTE: the component version is NOT the same 
        as the OMX Specification version (found in all structures).  The 
        component version is defined by the vendor of the component and 
        its value is entirely up to the component vendor.
    @param [out] pSpecVersion
        pointer to an OMX Version structure that the component will fill 
        in.  The SpecVersion is the version of the specification that the 
        component was built against.  Please note that this value may or 
        may not match the structure's version.  For example, if the 
        component was built against the 2.0 specification, but the 
        application (which creates the structure is built against the 
        1.0 specification the versions would be different.
    @param [out] pComponentUUID
        pointer to the UUID of the component which will be filled in by 
        the component.  The UUID is a unique identifier that is set at 
        RUN time for the component and is unique to each instantion of 
        the component.
    @return OMX_ERRORTYPE
        If the command successfully executes, the return code will be
        OMX_NoError.  Otherwise the appropriate OMX error will be returned.
 */
#define OMX_GetComponentVersion(                            \
        hComponent,                                         \
        pComponentName,                                     \
        pComponentVersion,                                  \
        pSpecVersion,                                       \
        pComponentUUID)                                     \
    ((OMX_COMPONENTTYPE*)hComponent)->GetComponentVersion(  \
        hComponent,                                         \
        pComponentName,                                     \
        pComponentVersion,                                  \
        pSpecVersion,                                       \
        pComponentUUID)                 /* Macro End */


/** Send a command to the component.  This call is a non-blocking call.
    The component should check the parameters and then queue the command
    to the component thread to be executed.  The component thread shall 
    send the EventHandler() callback at the conclusion of the command. 
    This macro will go directly from the application to the component (via
    a core macro).  The component will return from this call within 5 msec.
    
    When the command is "OMX_CommandStateSet" the component will queue a
    state transition to the new state idenfied in nParam.
    
    When the command is "OMX_CommandFlush", to flush a port's buffer queues,
    the command will force the component to return all buffers NOT CURRENTLY 
    BEING PROCESSED to the application, in the order in which the buffers 
    were received.
    
    When the command is "OMX_CommandPortDisable" or 
    "OMX_CommandPortEnable", the component's port (given by the value of
    nParam) will be stopped or restarted. 
    
    When the command "OMX_CommandMarkBuffer" is used to mark a buffer, the
    pCmdData will point to a OMX_MARKTYPE structure containing the component
    handle of the component to examine the buffer chain for the mark.  nParam1
    contains the index of the port on which the buffer mark is applied.

    Specification text for more details. 
    
    @param [in] hComponent
        handle of component to execute the command
    @param [in] Cmd
        Command for the component to execute
    @param [in] nParam
        Parameter for the command to be executed.  When Cmd has the value 
        OMX_SetStateCmd, value is a member of OMX_StateType.  When Cmd has 
        the value OMX_FlushCmd, value of nParam indicates which port(s) 
        to flush. -1 is used to flush all ports a single port index will 
        only flush that port.  When Cmd has the value "OMX_CommandPortDisable"
        or "OMX_CommandPortEnable", the component's port is given by 
        the value of nParam.  When Cmd has the value "OMX_CommandMarkBuffer"
        the components pot is given by the value of nParam.
    @param [in] pCmdData
        Parameter pointing to the OMX_MARKTYPE structure when Cmd has the value
        "OMX_CommandMarkBuffer".     
    @return OMX_ERRORTYPE
        If the command successfully executes, the return code will be
        OMX_NoError.  Otherwise the appropriate OMX error will be returned.
 */
#define OMX_SendCommand(                                    \
         hComponent,                                        \
         Cmd,                                               \
         nParam,                                            \
         pCmdData)                                          \
     ((OMX_COMPONENTTYPE*)hComponent)->SendCommand(         \
         hComponent,                                        \
         Cmd,                                               \
         nParam,                                            \
         pCmdData)                          /* Macro End */


/** The OMX_GetParameter macro will get one of the current parameter 
    settings from the component.  This macro cannot only be invoked when 
    the component is in the OMX_IndalidState state.  The nParamIndex
    parameter is used to indicate which structure is being requested from
    the component.  The application shall allocate the correct structure 
    and shall fill in the structure size and version information before 
    invoking this macro.  When the parameter applies to a port, the
    caller shall fill in the appropriate nPortIndex value indicating the
    port on which the parameter applies. If the component has not had 
    any settings changed, then the component should return a set of 
    valid DEFAULT  parameters for the component.  This is a blocking 
    call.  
    
    The component should return from this call within 20 msec.
    
    @param [in] hComponent
        Handle of the component to be accessed.  This is the component
        handle returned by the call to the OMX_GetHandle function.
    @param [in] nParamIndex
        Index of the structure to be filled.  This value is from the
        OMX_INDEXTYPE enumeration.
    @param [in,out] ComponentParameterStructure
        Pointer to application allocated structure to be filled by the 
        component.
    @return OMX_ERRORTYPE
        If the command successfully executes, the return code will be
        OMX_NoError.  Otherwise the appropriate OMX error will be returned.
 */
#define OMX_GetParameter(                                   \
        hComponent,                                         \
        nParamIndex,                                        \
        ComponentParameterStructure)                        \
    ((OMX_COMPONENTTYPE*)hComponent)->GetParameter(         \
        hComponent,                                         \
        nParamIndex,                                        \
        ComponentParameterStructure)    /* Macro End */


/** The OMX_SetParameter macro will send an initialization parameter
    structure to a component.  Each structure shall be sent one at a time,
    in a separate invocation of the macro.  This macro can only be
    invoked when the component is in the OMX_LoadedState state, or the
    port is disabled (when the parameter applies to a port). The 
    nParamIndex parameter is used to indicate which structure is being
    passed to the component.  The application shall allocate the 
    correct structure and shall fill in the structure size and version 
    information (as well as the actual data) before invoking this macro.
    The application is free to dispose of this structure after the call
    as the component is required to copy any data it shall retain.  This 
    is a blocking call.  
    
    The component should return from this call within 20 msec.
    
    @param [in] hComponent
        Handle of the component to be accessed.  This is the component
        handle returned by the call to the OMX_GetHandle function.
    @param [in] nIndex
        Index of the structure to be sent.  This value is from the
        OMX_INDEXTYPE enumeration.
    @param [in] ComponentParameterStructure
        pointer to application allocated structure to be used for
        initialization by the component.
    @return OMX_ERRORTYPE
        If the command successfully executes, the return code will be
        OMX_NoError.  Otherwise the appropriate OMX error will be returned.
 */
#define OMX_SetParameter(                                   \
        hComponent,                                         \
        nParamIndex,                                        \
        ComponentParameterStructure)                        \
    ((OMX_COMPONENTTYPE*)hComponent)->SetParameter(         \
        hComponent,                                         \
        nParamIndex,                                        \
        ComponentParameterStructure)    /* Macro End */


/** The OMX_GetConfig macro will get one of the configuration structures 
    from a component.  This macro can be invoked anytime after the 
    component has been loaded.  The nParamIndex call parameter is used to 
    indicate which structure is being requested from the component.  The 
    application shall allocate the correct structure and shall fill in the 
    structure size and version information before invoking this macro.  
    If the component has not had this configuration parameter sent before, 
    then the component should return a set of valid DEFAULT values for the 
    component.  This is a blocking call.  
    
    The component should return from this call within 5 msec.
    
    @param [in] hComponent
        Handle of the component to be accessed.  This is the component
        handle returned by the call to the OMX_GetHandle function.
    @param [in] nIndex
        Index of the structure to be filled.  This value is from the
        OMX_INDEXTYPE enumeration.
    @param [in,out] pComponentConfigStructure
        pointer to application allocated structure to be filled by the 
        component.
    @return OMX_ERRORTYPE
        If the command successfully executes, the return code will be
        OMX_NoError.  Otherwise the appropriate OMX error will be returned.
*/        
#define OMX_GetConfig(                                      \
        hComponent,                                         \
        nConfigIndex,                                       \
        ComponentConfigStructure)                           \
    ((OMX_COMPONENTTYPE*)hComponent)->GetConfig(            \
        hComponent,                                         \
        nConfigIndex,                                       \
        ComponentConfigStructure)       /* Macro End */


/** The OMX_SetConfig macro will send one of the configuration 
    structures to a component.  Each structure shall be sent one at a time,
    each in a separate invocation of the macro.  This macro can be invoked 
    anytime after the component has been loaded.  The application shall 
    allocate the correct structure and shall fill in the structure size 
    and version information (as well as the actual data) before invoking 
    this macro.  The application is free to dispose of this structure after 
    the call as the component is required to copy any data it shall retain.  
    This is a blocking call.  
    
    The component should return from this call within 5 msec.
    
    @param [in] hComponent
        Handle of the component to be accessed.  This is the component
        handle returned by the call to the OMX_GetHandle function.
    @param [in] nConfigIndex
        Index of the structure to be sent.  This value is from the
        OMX_INDEXTYPE enumeration above.
    @param [in] ComponentConfigStructure
        pointer to application allocated structure to be used for
        initialization by the component.
    @return OMX_ERRORTYPE
        If the command successfully executes, the return code will be
        OMX_NoError.  Otherwise the appropriate OMX error will be returned.
 */
#define OMX_SetConfig(                                      \
        hComponent,                                         \
        nConfigIndex,                                       \
        ComponentConfigStructure)                           \
    ((OMX_COMPONENTTYPE*)hComponent)->SetConfig(            \
        hComponent,                                         \
        nConfigIndex,                                       \
        ComponentConfigStructure)       /* Macro End */


/** The OMX_GetExtensionIndex macro will invoke a component to translate 
    a vendor specific configuration or parameter string into an OMX 
    structure index.  There is no requirement for the vendor to support 
    this command for the indexes already found in the OMX_INDEXTYPE 
    enumeration (this is done to save space in small components).  The 
    component shall support all vendor supplied extension indexes not found
    in the master OMX_INDEXTYPE enumeration.  This is a blocking call.  
    
    The component should return from this call within 5 msec.
    
    @param [in] hComponent
        Handle of the component to be accessed.  This is the component
        handle returned by the call to the GetHandle function.
    @param [in] cParameterName
        OMX_STRING that shall be less than 128 characters long including
        the trailing null byte.  This is the string that will get 
        translated by the component into a configuration index.
    @param [out] pIndexType
        a pointer to a OMX_INDEXTYPE to receive the index value.
    @return OMX_ERRORTYPE
        If the command successfully executes, the return code will be
        OMX_NoError.  Otherwise the appropriate OMX error will be returned.
 */
#define OMX_GetExtensionIndex(                              \
        hComponent,                                         \
        cParameterName,                                     \
        pIndexType)                                         \
    ((OMX_COMPONENTTYPE*)hComponent)->GetExtensionIndex(    \
        hComponent,                                         \
        cParameterName,                                     \
        pIndexType)                     /* Macro End */


/** The OMX_GetState macro will invoke the component to get the current 
    state of the component and place the state value into the location
    pointed to by pState.  
    
    The component should return from this call within 5 msec.
    
    @param [in] hComponent
        Handle of the component to be accessed.  This is the component
        handle returned by the call to the OMX_GetHandle function.
    @param [out] pState
        pointer to the location to receive the state.  The value returned
        is one of the OMX_STATETYPE members 
    @return OMX_ERRORTYPE
        If the command successfully executes, the return code will be
        OMX_NoError.  Otherwise the appropriate OMX error will be returned.
 */
#define OMX_GetState(                                       \
        hComponent,                                         \
        pState)                                             \
    ((OMX_COMPONENTTYPE*)hComponent)->GetState(             \
        hComponent,                                         \
        pState)                         /* Macro End */


/** The OMX_UseBuffer macro will request that the component use
    a buffer (and allocate its own buffer header) already allocated 
    by another component, or by the IL Client. This is a blocking 
    call.
    
    The component should return from this call within 20 msec.
    
    @param [in] hComponent
        Handle of the component to be accessed.  This is the component
        handle returned by the call to the OMX_GetHandle function.
    @param [out] ppBuffer
        pointer to an OMX_BUFFERHEADERTYPE structure used to receive the 
        pointer to the buffer header
    @return OMX_ERRORTYPE
        If the command successfully executes, the return code will be
        OMX_NoError.  Otherwise the appropriate OMX error will be returned.
 */

#define OMX_UseBuffer(                                      \
           hComponent,                                      \
           ppBufferHdr,                                     \
           nPortIndex,                                      \
           pAppPrivate,                                     \
           nSizeBytes,                                      \
           pBuffer)                                         \
    ((OMX_COMPONENTTYPE*)hComponent)->UseBuffer(            \
           hComponent,                                      \
           ppBufferHdr,                                     \
           nPortIndex,                                      \
           pAppPrivate,                                     \
           nSizeBytes,                                      \
           pBuffer)


/** The OMX_AllocateBuffer macro will request that the component allocate 
    a new buffer and buffer header.  The component will allocate the 
    buffer and the buffer header and return a pointer to the buffer 
    header.  This is a blocking call.
    
    The component should return from this call within 5 msec.
    
    @param [in] hComponent
        Handle of the component to be accessed.  This is the component
        handle returned by the call to the OMX_GetHandle function.
    @param [out] ppBuffer
        pointer to an OMX_BUFFERHEADERTYPE structure used to receive 
        the pointer to the buffer header
    @param [in] nPortIndex
        nPortIndex is used to select the port on the component the buffer will
        be used with.  The port can be found by using the nPortIndex
        value as an index into the Port Definition array of the component.
    @param [in] pAppPrivate
        pAppPrivate is used to initialize the pAppPrivate member of the 
        buffer header structure.
    @param [in] nSizeBytes
        size of the buffer to allocate.  Used when bAllocateNew is true.
    @return OMX_ERRORTYPE
        If the command successfully executes, the return code will be
        OMX_NoError.  Otherwise the appropriate OMX error will be returned.
 */    
#define OMX_AllocateBuffer(                                 \
        hComponent,                                         \
        ppBuffer,                                           \
        nPortIndex,                                         \
        pAppPrivate,                                        \
        nSizeBytes)                                         \
    ((OMX_COMPONENTTYPE*)hComponent)->AllocateBuffer(       \
        hComponent,                                         \
        ppBuffer,                                           \
        nPortIndex,                                         \
        pAppPrivate,                                        \
        nSizeBytes)                     /* Macro End */


/** The OMX_FreeBuffer macro will release a buffer header from the component
    which was allocated using either OMX_AllocateBuffer or OMX_UseBuffer. If  
    the component allocated the buffer (see the OMX_UseBuffer macro) then 
    the component shall free the buffer and buffer header. This is a 
    blocking call. 
    
    The component should return from this call within 20 msec.
    
    @param [in] hComponent
        Handle of the component to be accessed.  This is the component
        handle returned by the call to the OMX_GetHandle function.
    @param [in] nPortIndex
        nPortIndex is used to select the port on the component the buffer will
        be used with.
    @param [in] pBuffer
        pointer to an OMX_BUFFERHEADERTYPE structure allocated with UseBuffer
        or AllocateBuffer.
    @return OMX_ERRORTYPE
        If the command successfully executes, the return code will be
        OMX_NoError.  Otherwise the appropriate OMX error will be returned.
 */
#define OMX_FreeBuffer(                                     \
        hComponent,                                         \
        nPortIndex,                                         \
        pBuffer)                                            \
    ((OMX_COMPONENTTYPE*)hComponent)->FreeBuffer(           \
        hComponent,                                         \
        nPortIndex,                                         \
        pBuffer)                        /* Macro End */


/** The OMX_EmptyThisBuffer macro will send a buffer full of data to an 
    input port of a component.  The buffer will be emptied by the component
    and returned to the application via the EmptyBufferDone call back.
    This is a non-blocking call in that the component will record the buffer
    and return immediately and then empty the buffer, later, at the proper 
    time.  As expected, this macro may be invoked only while the component 
    is in the OMX_ExecutingState.  If nPortIndex does not specify an input
    port, the component shall return an error.  
    
    The component should return from this call within 5 msec.
    
    @param [in] hComponent
        Handle of the component to be accessed.  This is the component
        handle returned by the call to the OMX_GetHandle function.
    @param [in] pBuffer
        pointer to an OMX_BUFFERHEADERTYPE structure allocated with UseBuffer
        or AllocateBuffer.
    @return OMX_ERRORTYPE
        If the command successfully executes, the return code will be
        OMX_NoError.  Otherwise the appropriate OMX error will be returned.
 */
#define OMX_EmptyThisBuffer(                                \
        hComponent,                                         \
        pBuffer)                                            \
    ((OMX_COMPONENTTYPE*)hComponent)->EmptyThisBuffer(      \
        hComponent,                                         \
        pBuffer)                        /* Macro End */


/** The OMX_FillThisBuffer macro will send an empty buffer to an 
    output port of a component.  The buffer will be filled by the component
    and returned to the application via the FillBufferDone call back.
    This is a non-blocking call in that the component will record the buffer
    and return immediately and then fill the buffer, later, at the proper 
    time.  As expected, this macro may be invoked only while the component 
    is in the OMX_ExecutingState.  If nPortIndex does not specify an output
    port, the component shall return an error.  
    
    The component should return from this call within 5 msec.
    
    @param [in] hComponent
        Handle of the component to be accessed.  This is the component
        handle returned by the call to the OMX_GetHandle function.
    @param [in] pBuffer
        pointer to an OMX_BUFFERHEADERTYPE structure allocated with UseBuffer
        or AllocateBuffer.
    @return OMX_ERRORTYPE
        If the command successfully executes, the return code will be
        OMX_NoError.  Otherwise the appropriate OMX error will be returned.
 */
#define OMX_FillThisBuffer(                                 \
        hComponent,                                         \
        pBuffer)                                            \
    ((OMX_COMPONENTTYPE*)hComponent)->FillThisBuffer(       \
        hComponent,                                         \
        pBuffer)                        /* Macro End */


/** The OMX_Init method is used to initialize the OMX core.  It shall be the
    first call made into OMX and it should only be executed one time without
    an interviening OMX_Deinit call.  
    
    The core should return from this call within 20 msec.

    @return OMX_ERRORTYPE
        If the command successfully executes, the return code will be
        OMX_NoError.  Otherwise the appropriate OMX error will be returned.
 */
OMX_API OMX_ERRORTYPE OMX_APIENTRY OMX_Init(void);


/** The OMX_Deinit method is used to deinitialize the OMX core.  It shall be 
    the last call made into OMX. In the event that the core determines that 
    thare are components loaded when this call is made, the core may return 
    with an error rather than try to unload the components.
        
    The core should return from this call within 20 msec.
    
    @return OMX_ERRORTYPE
        If the command successfully executes, the return code will be
        OMX_NoError.  Otherwise the appropriate OMX error will be returned.
 */
OMX_API OMX_ERRORTYPE OMX_APIENTRY OMX_Deinit(void);


/** The OMX_ComponentNameEnum method will enumerate through all the names of
    recognised valid components in the system. This function is provided
    as a means to detect all the components in the system run-time. There is
    no strict ordering to the enumeration order of component names, although
    each name will only be enumerated once.  If the OMX core supports run-time
    installation of new components, it is only requried to detect newly
    installed components when the first call to enumerate component names
    is made (i.e. when nIndex is 0x0).
    
    The core should return from this call in 20 msec.
    
    @param [out] cComponentName
        pointer to a null terminated string with the component name.  The
        names of the components are strings less than 127 bytes in length
        plus the trailing null for a maximum size of 128 bytes.  An example 
        of a valid component name is "OMX.TI.AUDIO.DSP.MIXER\0".  Names are 
        assigned by the vendor, but shall start with "OMX." and then have 
        the Vendor designation next.
    @param [in] nNameLength
        number of characters in the cComponentName string.  With all 
        component name strings restricted to less than 128 characters 
        (including the trailing null) it is recomended that the caller
        provide a input string for the cComponentName of 128 characters.
    @param [in] nIndex
        number containing the enumeration index for the component. 
        Multiple calls to OMX_ComponentNameEnum with increasing values
        of nIndex will enumerate through the component names in the
        system until OMX_ErrorNoMore is returned.  The value of nIndex
        is 0 to (N-1), where N is the number of valid installed components
        in the system.
    @return OMX_ERRORTYPE
        If the command successfully executes, the return code will be
        OMX_NoError.  When the value of nIndex exceeds the number of 
        components in the system minus 1, OMX_ErrorNoMore will be
        returned. Otherwise the appropriate OMX error will be returned.
 */
OMX_API OMX_ERRORTYPE OMX_APIENTRY OMX_ComponentNameEnum(
    OMX_OUT OMX_STRING cComponentName,
    OMX_IN  OMX_U32 nNameLength,
    OMX_IN  OMX_U32 nIndex);


/** The OMX_GetHandle method will locate the component specified by the
    component name given, load that component into memory and then invoke
    the component's methods to create an instance of the component.  
    
    The core should return from this call within 20 msec.
    
    @param [out] pHandle
        pointer to an OMX_HANDLETYPE pointer to be filled in by this method.
    @param [in] cComponentName
        pointer to a null terminated string with the component name.  The
        names of the components are strings less than 127 bytes in length
        plus the trailing null for a maximum size of 128 bytes.  An example 
        of a valid component name is "OMX.TI.AUDIO.DSP.MIXER\0".  Names are 
        assigned by the vendor, but shall start with "OMX." and then have 
        the Vendor designation next.
    @param [in] pAppData
        pointer to an application defined value that will be returned
        during callbacks so that the application can identify the source
        of the callback.
    @param [in] pCallBacks
        pointer to a OMX_CALLBACKTYPE structure that will be passed to the
        component to initialize it with.  
    @return OMX_ERRORTYPE
        If the command successfully executes, the return code will be
        OMX_NoError.  Otherwise the appropriate OMX error will be returned.
 */
OMX_API OMX_ERRORTYPE OMX_APIENTRY OMX_GetHandle(
    OMX_OUT OMX_HANDLETYPE* pHandle, 
    OMX_IN  OMX_STRING cComponentName,
    OMX_IN  OMX_PTR pAppData,
    OMX_IN  OMX_CALLBACKTYPE* pCallBacks);


/** The OMX_FreeHandle method will free a handle allocated by the OMX_GetHandle 
    method.  If the component reference count goes to zero, the component will
    be unloaded from memory.  
    
    The core should return from this call within 20 msec when the component is 
    in the OMX_StateLoaded state.

    @param [in] hComponent
        Handle of the component to be accessed.  This is the component
        handle returned by the call to the GetHandle function.
    @return OMX_ERRORTYPE
        If the command successfully executes, the return code will be
        OMX_NoError.  Otherwise the appropriate OMX error will be returned.
 */
OMX_API OMX_ERRORTYPE OMX_APIENTRY OMX_FreeHandle(
    OMX_IN  OMX_HANDLETYPE hComponent);



/** The OMX_SetupTunnel method will handle the necessary calls to the components
    to setup the specified tunnel the two components.  NOTE: This is
    an actual method (not a #define macro).  This method will make calls into
    the component ComponentTunnelRequest method to do the actual tunnel 
    connection.  

    The ComponentTunnelRequest method on both components will be called. 
    This method shall not be called unless the component is in the 
    OMX_StateLoaded state except when the ports used for the tunnel are
    disabled. In this case, the component may be in the OMX_StateExecuting,
    OMX_StatePause, or OMX_StateIdle states. 

    The core should return from this call within 20 msec.
    
    @param [in] hOutput
        Handle of the component to be accessed.  Also this is the handle
        of the component whose port, specified in the nPortOutput parameter
        will be used the source for the tunnel. This is the component handle
        returned by the call to the OMX_GetHandle function.  There is a 
        requirement that hOutput be the source for the data when
        tunelling (i.e. nPortOutput is an output port).  If 0x0, the component
        specified in hInput will have it's port specified in nPortInput
        setup for communication with the application / IL client.
    @param [in] nPortOutput
        nPortOutput is used to select the source port on component to be
        used in the tunnel. 
    @param [in] hInput
        This is the component to setup the tunnel with. This is the handle
        of the component whose port, specified in the nPortInput parameter
        will be used the destination for the tunnel. This is the component handle
        returned by the call to the OMX_GetHandle function.  There is a 
        requirement that hInput be the destination for the data when
        tunelling (i.e. nPortInut is an input port).   If 0x0, the component
        specified in hOutput will have it's port specified in nPortPOutput
        setup for communication with the application / IL client.
    @param [in] nPortInput
        nPortInput is used to select the destination port on component to be
        used in the tunnel.
    @return OMX_ERRORTYPE
        If the command successfully executes, the return code will be
        OMX_NoError.  Otherwise the appropriate OMX error will be returned.
        When OMX_ErrorNotImplemented is returned, one or both components is 
        a non-interop component and does not support tunneling.
        
        On failure, the ports of both components are setup for communication
        with the application / IL Client.
 */
OMX_API OMX_ERRORTYPE OMX_APIENTRY OMX_SetupTunnel(
    OMX_IN  OMX_HANDLETYPE hOutput,
    OMX_IN  OMX_U32 nPortOutput,
    OMX_IN  OMX_HANDLETYPE hInput,
    OMX_IN  OMX_U32 nPortInput);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
/* File EOF */

