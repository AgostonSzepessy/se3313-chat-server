import sys
from PyQt5 import QtWidgets, QtGui, QtCore
from multiprocessing.pool import ThreadPool
import socket



class Window(QtWidgets.QMainWindow):
    
    def __init__(self, controller):
        super().__init__()

        self.controller = controller  # set the controller reference

        # set basic window properties
        self.setGeometry(200, 200, 600, 400) # left, top, width, height
        self.setFixedSize(600, 450) # force window to be fixed size (cant maximize, resize)
        self.setWindowTitle("Chat Application")   
     
        self.messages = [] # will later hold references to all the messages on the screen (MAX 16 on screen at a time)

        self.init_ui() # call to init the build UI
    
    def init_ui(self):
        # build widgets
        self.mainWidget = QtWidgets.QWidget()
        self.setCentralWidget(self.mainWidget)
        self.mainWidgetLayout = QtWidgets.QVBoxLayout()
        self.mainWidget.setLayout(self.mainWidgetLayout)

        self.messageCreateWidget = QtWidgets.QHBoxLayout()

        self.messageCreateText = QtWidgets.QLineEdit()        
        
        self.messageCreateSendButton = QtWidgets.QPushButton()
        self.messageCreateSendButton.setText("Send")
        self.messageCreateSendButton.setMaximumWidth(100)
        self.messageCreateSendButton.clicked.connect(self.sendMessage)

        self.messageCreateWidget.addWidget(self.messageCreateText)
        self.messageCreateWidget.addWidget(self.messageCreateSendButton)

        self.messageDisplayWidget = QtWidgets.QStackedLayout()
        self.messageDisplayLayout = QtWidgets.QVBoxLayout()
        self.messageDisplay = QtWidgets.QScrollArea()
        self.messageDisplay.setWidgetResizable(True)
        self.messageDisplay.setFixedHeight(400)
        self.messageDisplay.setLayout(self.messageDisplayLayout)
        self.messageDisplayLayout.addStretch(20)
        self.messageDisplayWidget.addWidget(self.messageDisplay)

        self.mainWidgetLayout.insertLayout(0, self.messageCreateWidget, 1)
        self.mainWidgetLayout.insertLayout(0, self.messageDisplayWidget, 9)

        # show the window
        self.show()
    
    def sendMessage(self):
        # get message text
        text = self.messageCreateText.text()
        # dispatch controller
        self.controller.sendMessage(text)

    def printMessage(self, message, sentFromUser=True):
        # print the message
        if message is None or message == "":
            pass

        else:
            
            if len(self.messages) > 16:
                # screen is full so clear oldest msg
                oldestMessage = self.messages.pop(0)
                self.messageDisplayLayout.removeWidget(oldestMessage)
                oldestMessage.deleteLater()

            newMessage = QtWidgets.QLabel(message)
            self.messages.append(newMessage)
            
            if sentFromUser:
                self.messageDisplayLayout.insertWidget(len(self.messages)+1, self.messages[-1:][0], 1, QtCore.Qt.AlignRight)
            else:
                self.messageDisplayLayout.insertWidget(len(self.messages)+1, self.messages[-1:][0], 1, QtCore.Qt.AlignLeft)
        return


class Controller:
        def __init__(self):
            # init the GUI
            self.app = QtWidgets.QApplication(sys.argv)
            self.window = Window(self)
            self.messenger = SocketMessenger(self)
        
        def run(self):
            # start the GUI
            sys.exit(self.app.exec_())

        def sendMessage(self, message):
            if message is None or message == "":
                return
            print("Sending message: %s"%message)
            self.window.printMessage(message, True)
            # use socket to send message
            self.messenger.sendMessage(message)

        def receiveMessage(self, message):
            print("Receiving message: %s to %s"%(message, self.window))
            self.window.printMessage(message, False)

class Listener(QtCore.QThread):
    signal = QtCore.pyqtSignal(object) # signal event object

    def __init__(self, messenger):
        super().__init__()
        self.messenger = messenger
        
    def run(self):
        while True:
            msg = self.messenger.socket.recv(1024).decode('ascii') # receive the message
            self.signal.emit(msg) # signal event on main thread to dispatch


class SocketMessenger:
        def __init__(self, controller):
            self.controller = controller
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            
            self.connect()

            self.listener = Listener(self)
            self.listener.signal.connect(self.receiveMessage)
            self.listener.start()

        def connect(self):
            # self.socket.connect_ex(('172.31.51.241', 2004))
            print("Connecting: %s"%self.socket.connect_ex(('127.0.0.1', 2004)))

        def sendMessage(self, message):
            self.socket.send(message.encode('ascii'))
        
        def receiveMessage(self, message):
            if message.startswith('||'):
                # regular message, print to screen
                self.controller.receiveMessage(message[2:])
            else:
                # special server message

                if message == 'a':
                    # client is first in chat room
                    self.controller.receiveMessage("Please wait for another user to join the chat room")
                elif message == '1':
                    # chat room initialized
                    self.controller.receiveMessage("Chat room open")
                else:
                    # quit message
                    self.controller.receiveMessage("Other user left the chat room")

controller = Controller()
controller.run()
