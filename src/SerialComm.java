
import jssc.*;

public class SerialComm {

	SerialPort port;

	private boolean debug;  // Indicator of "debugging mode"
	
	// This function can be called to enable or disable "debugging mode"
	void setDebug(boolean mode) {
		debug = mode;
	}	
	

	// Constructor for the SerialComm class
	public SerialComm(String name) throws SerialPortException {
		port = new SerialPort(name);		
		port.openPort();
		port.setParams(SerialPort.BAUDRATE_9600,
			SerialPort.DATABITS_8,
			SerialPort.STOPBITS_1,
			SerialPort.PARITY_NONE);
		
		debug = false; // Default is to NOT be in debug mode
	}
		
	// TODO: Add writeByte() method from Studio 5
	public void writeByte(byte singleByte)
	{
		//byte singleByte = ap.nextByte();
		try {
			port.writeByte(singleByte);
		} catch (SerialPortException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		if (debug)
			System.out.println(singleByte);
	}
	
	// TODO: Add available() method
	public boolean available() {
		try {
			if(port.getInputBufferBytesCount()!=-1)
				return true;
			else
				return false;
		} catch (SerialPortException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return false;
	}
	// TODO: Add readByte() method	
	public void readByte() throws SerialPortException {
		byte[] c = new byte[1];
		c = port.readBytes(1);
		System.out.println((char) c[0]);
	}
	
	// TODO: Add a main() method
}
