import jssc.SerialPortException;

public class PCPlayer{
	public static void main(String[] args)
	{
		int moveRate = 100;
		long readTime = 0;
		SerialComm port;
		try {
			port = new SerialComm("/dev/cu.usbserial-DN02AZZ5");
			while (true)
			{
				if (System.currentTimeMillis() - readTime > 2 * moveRate)
				{
					if (StdDraw.isKeyPressed(37))
					{
						port.writeByte((byte) 'l');
						System.out.println("Left!");
						readTime = System.currentTimeMillis();
					}
					else if (StdDraw.isKeyPressed(39))
					{
						port.writeByte((byte) 'r');
						System.out.println("Right!");
						readTime = System.currentTimeMillis();
					}
					else if (StdDraw.isKeyPressed(38))
					{
						port.writeByte((byte) 'u');
						System.out.println("Up!");
						readTime = System.currentTimeMillis();
					}
					else if (StdDraw.isKeyPressed(40))
					{
						port.writeByte((byte) 'd');
						System.out.println("Down!");
						readTime = System.currentTimeMillis();
					}
				}
			}
		} catch (SerialPortException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}