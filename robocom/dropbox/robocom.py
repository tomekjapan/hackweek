import serial, time

class SerialStream:
    def __init__ (self):
        self.device = '/dev/ttyUSB0'
        self.port = serial.Serial(
            port=self.device,
            baudrate=57600,
            timeout=0)
        # Must sleep so that the board can reset
        if self.device.find('ACM') > 0:
            print "Waiting for 2 seconds..."
            time.sleep(2)
            print "done"
        self.peekedByte = -1
        self.totalRead = 0

    def available (self):
        num = self.port.inWaiting()
        if self.peekedByte >= 0:
            num = num + 1
        return num

    def peek (self):
        if self.peekedByte < 0:
            self.peekedByte = self.read()
        return self.peekedByte

    def read (self):
        bs = self.readBytes(1)
        if len(bs) == 0:
            return -1
        return bs[0]

    def readBytes (self, size):
        bs = bytearray()
        if size == 0:
            return b
        if self.peekedByte >= 0:
            bs.append( self.peekedByte )
            self.peekedByte = -1
        if size > len(bs):
            newbs = self.port.read(size - len(bs))
            #print "Got", map(lambda x: ord(x), newbs)
            self.totalRead += len(newbs)
            bs.extend(newbs)
        return bs

    def write (self, b):
        self.port.write( bytearray(b) )

    def writeBytes (self, bs):
        self.port.write(b)

# Message types
MSGID_NOOP = 0
MSGID_ECHO = 1
MSGID_RESET = 2
MSGID_FLUSH = 3
MSGID_WHEEL_DRIVE = 4
MSGID_ENCODER_READING = 5
MSGID_SERVO_ANGLE = 6


MSG_START = ord('>')
MSG_END = ord('<')

MSG_HEADER_SIZE = 4
MSG_MAX_DATA_SIZE = 16

def serializeInt2 (v):
    return [ v & 0xff, (v >> 8) & 0xff ]

def deserializeInt2 (bs, offset):
    return bs[offset] | (bs[offset+1] << 8)

def deserializeInt4 (bs, offset):
    return bs[offset] | (bs[offset+1] << 8) | \
        (bs[offset+2] << 16) | (bs[offset+3] << 24)

NEXT_TASK_ID = 1
def nextTaskId ():
    global NEXT_TASK_ID
    id = NEXT_TASK_ID
    NEXT_TASK_ID = NEXT_TASK_ID + 1
    return id

class NoopRequest:
    def __init__ (self):
        self.taskId = nextTaskId()
    def serialize (self):
        bs = [4, MSGID_NOOP | 0x80]
        bs.extend(serializeInt2(self.taskId))
        return bs
    def __str__ (self):
        return "NoopRequest(taskId=%d)" % self.taskId

class EchoRequest:
    def __init__ (self):
        self.taskId = nextTaskId()
    def serialize (self):
        bs = [4, MSGID_ECHO | 0x80]
        bs.extend(serializeInt2(self.taskId))
        return bs
    def __str__ (self):
        return "EchoRequest(taskId=%d)" % self.taskId

class ResetRequest:
    def __init__ (self):
        self.taskId = nextTaskId()
    def serialize (self):
        bs = [4, MSGID_RESET | 0x80]
        bs.extend(serializeInt2(self.taskId))
        return bs
    def __str__ (self):
        return "ResetRequest(taskId=%d)" % self.taskId

class FlushRequest:
    def __init__ (self):
        self.taskId = nextTaskId()
    def serialize (self):
        bs = [4, MSGID_FLUSH | 0x80]
        bs.extend(serializeInt2(self.taskId))
        return bs
    def __str__ (self):
        return "FlushRequest(taskId=%d)" % self.taskId

class SetWheelDriveRequest:
    def __init__ (self, m1dir, m1sig, m2dir, m2sig):
        self.taskId = nextTaskId()
        self.m1dir = m1dir
        self.m1sig = m1sig
        self.m2dir = m2dir
        self.m2sig = m2sig
    def serialize (self):
        bs = [8, MSGID_WHEEL_DRIVE | 0x80]
        bs.extend(serializeInt2(self.taskId))
        bs.extend([self.m1dir, self.m1sig, self.m2dir, self.m2sig])
        return bs

class SetServoAngleRequest:
    def __init__ (self, servoId, angle):
        self.taskId = nextTaskId()
        self.servoId = servoId
        self.angle = angle
    def serialize (self):
        bs = [6, MSGID_SERVO_ANGLE | 0x80]
        bs.extend(serializeInt2(self.taskId))
        bs.extend([self.servoId, self.angle])
        return bs

class EncoderReadingRequest:
    def __init__ (self, encoderId, isSubscribe):
        self.taskId = nextTaskId()
        self.encoderId = encoderId
        self.isSubscribe = isSubscribe
    def serialize (self):
        bs = [6, MSGID_ENCODER_READING | 0x80]
        bs.extend(serializeInt2(self.taskId))
        bs.extend([self.encoderId, 1 if self.isSubscribe else 0])
        return bs

class Response:
    def __init__ (self, taskId, isImmediate, data):
        self.taskId = taskId
        self.isImmediate = isImmediate
        self.offset = 0 if isImmediate else 4
        self.millis = 0 if isImmediate else deserializeInt4(data,0)

class EchoResponse (Response):
    def __init__ (self, taskId, isImmediate, data):
        Response.__init__(self, taskId, isImmediate, data)
    def __str__ (self):
        return "EchoResponse(taskId=%d, isImmediate=%d, millis=%d)" \
            % (self.taskId, self.isImmediate, self.millis)

class FlushResponse (Response):
    def __init__ (self, taskId, isImmediate, data):
        Response.__init__(self, taskId, isImmediate, data)
        self.minFreeSlots = data[self.offset]
        self.freeSlots = data[self.offset+1]
        self.inputQueueMaxSize = data[self.offset+2]
        self.inputQueueSize = data[self.offset+3]
        self.outputQueueMaxSize = data[self.offset+4]
        self.outputQueueSize = data[self.offset+5]
    def __str__ (self):
        return "FlushResponse(taskId=%d, isImmediate=%d, millis=%d, " \
            "slots(%d, %d), input(%d, %d), output(%d, %d)" \
            % (self.taskId, self.isImmediate, self.millis,
               self.minFreeSlots, self.freeSlots,
               self.inputQueueMaxSize, self.inputQueueSize,
               self.outputQueueMaxSize, self.outputQueueSize)

class WheelDriveChangedNotice (Response):
    def __init__ (self, taskId, isImmediate, data):
        Response.__init__(self, taskId, isImmediate, data)
        self.m1dir = data[self.offset]
        self.m1sig = data[self.offset+1]
        self.m2dir = data[self.offset+2]
        self.m2sig = data[self.offset+3]
    def __str__ (self):
        return "WheelDriveChangedNotice(taskId=%d, isImmediate=%d, millis=%d, " \
            "motor1=(dir=%d, sig=%d), motor2=(dir=%d, sig=%d)" \
            % (self.taskId, self.isImmediate, self.millis,
               self.m1dir, self.m1sig,
               self.m2dir, self.m2sig)

class EncoderReadingNotice (Response):
    def __init__(self, taskId, isImmediate, data):
        Response.__init__(self, taskId, isImmediate, data)
        self.encoderId = data[self.offset]
        self.tickIndex = deserializeInt4(data, self.offset+1)
        self.micros = deserializeInt4(data, self.offset+5)
    def __str__ (self):
        return "EncoderReadingNotice(taskId=%d, isImmediate=%d, millis=%d, " \
            "id=%d, tick=%d, micros=%d" \
            % (self.taskId, self.isImmediate, self.millis,
               self.encoderId, self.tickIndex, self.micros)

IOS_NEED_MESSAGE_START = 0
IOS_NEED_DATA = 1
IOS_HAVE_DATA = 2

class MessageIO:
    def __init__ (self, stream):
        self.stream = stream
        self.state = IOS_NEED_MESSAGE_START

    # Reads a message, returns the message or None if no message was available
    def read (self):
        if IOS_NEED_MESSAGE_START == self.state:
            if not self.readMessageStart():
                return None
        if IOS_NEED_DATA == self.state:
            if not self.waitForDataAvailable():
                return None
        return self.readData()

    # Writes a message
    def write (self, msg):
        self.stream.write(
            bytearray([MSG_START] + msg.serialize() + [MSG_END]) )

    def readMessageStart (self):
        while self.stream.available() > 0:
            if MSG_START == self.stream.read():
                self.state = IOS_NEED_DATA
                return True
        return False

    def waitForDataAvailable (self):
        if 0 == self.stream.available():
            return False
        msg_size = self.stream.peek()
        if msg_size > MSG_HEADER_SIZE + MSG_MAX_DATA_SIZE or \
                msg_size < MSG_HEADER_SIZE:
            self.state = IOS_NEED_MESSAGE_START
            return False
        if self.stream.available() <= msg_size:
            return False
        self.state = IOS_HAVE_DATA
        return True

    def readData (self):
        self.state = IOS_NEED_MESSAGE_START
        msg = self.deserializeMessage()
        if MSG_END == self.stream.read():
            return msg
        return None

    def deserializeMessage (self):
        msg_size = self.stream.read()
        data_size = msg_size - MSG_HEADER_SIZE
        c = self.stream.read()
        msg_type = c & 0x7F
        isImmediate = 1 if (c & 0x80) else 0
        task_id = self.stream.read() | (self.stream.read() << 8)
        data = self.stream.readBytes(data_size) if data_size > 0 else None
        if MSGID_ECHO == msg_type:
            return EchoResponse(task_id, isImmediate, data)
        if MSGID_FLUSH == msg_type:
            return FlushResponse(task_id, isImmediate, data)
        if MSGID_WHEEL_DRIVE == msg_type:
            return WheelDriveChangedNotice(task_id, isImmediate, data)
        if MSGID_ENCODER_READING == msg_type:
            return EncoderReadingNotice(task_id, isImmediate, data)

class Client:

    def __init__(self):
        self.ss = SerialStream()
        self.io = MessageIO(self.ss)

    def subscribe (self, id):
        self.io.write( EncoderReadingRequest(id, True) )

    def unsubscribe (self, id):
        self.io.write( EncoderReadingRequest(id, False) )

    def setDrive (self, m1dir, m1sig, m2dir, m2sig):
        self.io.write( SetWheelDriveRequest( m1dir, m1sig, m2dir, m2sig ) )

    def setServoAngle (self, angle):
        print "Angle ->", angle
        self.io.write( SetServoAngleRequest( 0, angle ) )

    def reset (self):
        self.io.write( ResetRequest() )

    def flush (self):
        req = FlushRequest()
        self.io.write( req )

        startTime = time.time()
        startRead = self.ss.totalRead
        msg = None
        while msg is None or msg.taskId != req.taskId:
            if time.time() - startTime > 2.0:
                return None
            msg = self.io.read()
            if not msg is None:
                print msg
                print "read", (self.ss.totalRead - startRead), "bytes, " \
                    "available", self.ss.available()

    def echo (self):
        self.io.write( EchoRequest() )
        msg = None
        while not msg:
            msg = self.io.read()
        print msg

def spinMe ():

    client = Client()

    # An "echo" message
    client.echo()

    client.reset()
    client.flush()

    client.subscribe(0)
    client.subscribe(1)

    client.setDrive( 0, 100, 0, 100 )
    client.flush()

    start = time.time()
    while time.time() - start < 3.0:
        time.sleep(0.4)
        client.flush()

    client.reset()
    client.flush()

def moveMyServo ():

    client = Client()
    client.echo()
    client.reset()
    client.flush()

    for i in range(30,181,30):
        client.setServoAngle(i)
        time.sleep(0.5)

    client.reset()
    client.flush()

if __name__ == '__main__':
    moveMyServo()

