#! /usr/bin/env python
import math, wx, Queue, threading
import robocom

def clamp (v, a, b):
    if v < a: return a
    elif v > b: return b
    else: return v

class DrawingWindow (wx.Window):

    def __init__ (self, parent):
        super(DrawingWindow, self).__init__(
            parent, style=wx.NO_FULL_REPAINT_ON_RESIZE, size=(300,200))
        self.SetBackgroundColour('White')
        self.Bind(wx.EVT_LEFT_DOWN, self.onLeftDown)
        self.Bind(wx.EVT_RIGHT_DOWN, self.reset)
        self.Bind(wx.EVT_LEFT_UP, self.onLeftUp)
        self.Bind(wx.EVT_MOTION, self.onMotion)
        self.Bind(wx.EVT_PAINT, self.onPaint)
        self.Bind(wx.EVT_SIZE, self.onSize)
        self.buttonDown = False
        self.position = (0,0)
        self.markerSize = 5
        self.radius = 1
        self.requestHandler = parent
        self.sqrt2o2 = math.sqrt(2) / 2

    def setDrive (self, m1, m2):
        f = self.radius * self.sqrt2o2
        self.setPosition(m1*f - m2*f, m1*f + m2*f)

    def requestDrive (self, m1, m2):
        self.requestHandler.onRequestDrive(m1, m2)
            
    def setPosition (self, x, y):
        if self.position[0] != x and self.position[1] != y:
            self.position = (x,y)
            self.Refresh()

    def requestPosition (self, x, y):
        f = 1.0 / self.radius
        m1 = clamp(x*f + y*f, -1.0, 1.0) * self.sqrt2o2
        m2 = clamp(-x*f + y*f, -1.0, 1.0) * self.sqrt2o2
        self.requestDrive(m1, m2)

    def reset (self, *args):
        self.requestPosition(0, 0)

    def onLeftDown(self, event):
        self.buttonDown = True
        self.requestPosition(
            *self.fromWindow(*event.GetPositionTuple()))

    def onLeftUp(self, event):
        self.buttonDown = False
        self.Refresh()

    def onMotion(self, event):
        if self.buttonDown:
            self.requestPosition(
                *self.fromWindow(*event.GetPositionTuple()))

    def onPaint(self, event):
        dc = wx.PaintDC(self)
        dc.BeginDrawing()
        self.drawDiamond(dc)
        self.drawSpring(dc)
        self.drawMarker(dc)
        dc.EndDrawing()

    def onSize (self, event):
        w, h = self.GetClientSizeTuple()
        oldr = self.radius
        self.radius = min(w,h) * 4 / 9
        self.position = map(
            lambda x: x * self.radius / oldr, self.position )
        self.Refresh()

    def drawMarker (self, dc):
        wxm, wym = self.toWindow( *self.position )
        dc.SetBackground(wx.Brush(self.GetBackgroundColour()))
        dc.DrawCircle( wxm, wym, self.markerSize )

    def drawSpring (self, dc):
        wxc, wyc = self.toWindow( 0, 0 )
        wxm, wym = self.toWindow( *self.position )
        dc.DrawLine(wxc, wyc, wxm, wym)

    def drawDiamond (self, dc):
        wxc, wyc = self.toWindow( 0, 0 )
        r = self.radius
        pts = [(-r,0), (0,r), (r,0), (0,-r)]
        for i in range(0,4):
            a = pts[i]
            b = pts[(i + 1) % len(pts)]
            dc.DrawLine(wxc + a[0], wyc + a[1],
                        wxc + b[0], wyc + b[1])
        for i in range(0,2):
            a = pts[i]
            b = pts[(i + 2) % len(pts)]
            dc.DrawLine(wxc + a[0], wyc + a[1],
                        wxc + b[0], wyc + b[1])

    def markerHit(self, wx, wy):
        x, y = self.fromWindow(wx, wy);
        xm, ym = self.position
        if x < xm - self.markerSize or xm + self.markerSize < x:
            return False
        if y < ym - self.markerSize or ym + self.markerSize < y:
            return False
        return True

    def toWindow (self, x, y):
        w, h = self.GetClientSizeTuple()
        return x + w/2, y + h/2

    def fromWindow (self, wx, wy):
        w, h = self.GetClientSizeTuple()
        return wx - w/2, wy - h/2


class Robocom:
    def __init__ (self):
        # Queue for incoming messages from the robot
        self.inqueue = Queue.Queue()
        # Queue for outgoing messages to the robot
        self.outqueue = Queue.Queue()
        # The serial port
        self.ss = robocom.SerialStream()
        # Serial port wrapper for the protocol IO
        self.io = robocom.MessageIO(self.ss)
        # Thread that handles the communication
        self.worker = threading.Thread(target=self.run)
        self.worker.start()

    def quit (self):
        print "quitting"
        self.outqueue.put("quit")
        self.worker.join()

    # Places the message in the output queue
    def push (self, msg):
        print "Outgoing:", msg
        self.outqueue.put(msg)

    # Try retrieving one message received from the robot
    def pop (self):
        try:
            msg = self.inqueue.get(False)
            return msg
        except Queue.Empty:
            return None

    def run (self):
        while True:
            try:
                msg = self.outqueue.get(True, timeout=0.1)
                if msg == "quit":
                    print "QUIT"
                    return
                self.io.write(msg)
            except Queue.Empty:
                self.flush()

    def setDrive(self, m1dir, m1sig, m2dir, m2sig):
        self.io.write(
            robocom.SetWheelDriveRequest( m1dir, m1sig, m2dir, m2sig ) )

    def reset (self):
        self.io.write( robocom.ResetRequest() )

    def flush (self):
        req = robocom.FlushRequest()
        self.io.write(req)
        msg = None
        while msg is None or msg.taskId != req.taskId:
            msg = self.io.read()
            if not msg is None:
                self.inqueue.put(msg)


class RCFrame (wx.Frame):

    def __init__ (self, parent, title):

        wx.Frame.__init__(self, parent, title=title)

        self.robocom = Robocom()
        self.Bind(wx.EVT_IDLE, self.onIdle)

        self.control = DrawingWindow(self)
        self.Bind(wx.EVT_CLOSE, self.onClose)

        filemenu = wx.Menu()

        menuAbout = filemenu.Append(
            wx.ID_ABOUT, "&About", " Information about this program")
        filemenu.AppendSeparator()
        menuExit = filemenu.Append(
            wx.ID_EXIT, "E&xit", " Terminate the program")

        menuBar = wx.MenuBar()
        menuBar.Append(filemenu, "&File")
        self.SetMenuBar(menuBar)

        self.Bind(wx.EVT_MENU, self.onAbout, menuAbout)
        self.Bind(wx.EVT_MENU, self.onExit, menuExit)

        self.resetButton = wx.Button(self, -1, "Reset")

        self.buttonSizer = wx.BoxSizer(wx.HORIZONTAL)
        self.buttonSizer.Add(self.resetButton)

        self.Bind(wx.EVT_BUTTON, self.onRequestReset, self.resetButton)

        self.sizer = wx.BoxSizer(wx.VERTICAL)
        self.sizer.Add(self.control, 1, wx.EXPAND)
        self.sizer.Add(self.buttonSizer, 0, wx.EXPAND)

        self.SetSizer(self.sizer)
        self.SetAutoLayout(1)
        self.sizer.Fit(self)
        self.Show()

    def onAbout (self, event):
        dlg = wx.MessageDialog(
            self, "Hack Week remote control", "About", wx.OK)
        dlg.ShowModal()
        dlg.Destroy()

    def onClose (self, event):
        self.robocom.quit()
        self.Destroy()

    def onExit (self, event):
        self.Close(True)

    def onRequestReset (self, event):
        self.robocom.push( robocom.ResetRequest() )

    # Requests wheel drive; the values for m1 and m2 are between
    # 0 and 1
    def onRequestDrive (self, m1, m2):
        m1dir = 0 if m1 >= 0 else 1
        m1sig = int(abs(m1*255))
        m2dir = 0 if m2 >= 0 else 1
        m2sig = int(abs(m2*255))
        self.robocom.push(
            robocom.SetWheelDriveRequest(m1dir, m1sig, m2dir, m2sig) )

    def onIdle (self, event):
        msg = self.robocom.pop()
        if msg is None:
            return
        print "Incoming:", msg
        if isinstance(msg, robocom.WheelDriveChangedNotice):
            return self.onDriveChanged(msg)

    # Notification from the robot that the drive has changed
    def onDriveChanged (self, msg):
        # This will be called when a message is received from the robot
        m1 = float(msg.m1sig) / 255
        if msg.m1dir > 0: m1 = -m1
        m2 = float(msg.m2sig) / 255
        if msg.m2dir > 0: m2 = -m2
        self.control.setDrive(m1, m2)

app = wx.App(False)
frame = RCFrame(None, "Hack Week")
app.MainLoop()
