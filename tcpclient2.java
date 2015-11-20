import java.io.*;
import java.net.*;

public class tcpclient2 {
	public static void main(String args[]) throws Exception {
		InetAddress host;
		host = InetAddress.getLocalHost();
		Client c = new Client(host);

		c.setSendPort(9002);
		c.setRecPort(8001);
		c.recData();
		c.sendData();
		c.recData();
	}
}

class Client {
	InetAddress host;
	int sendPort, recPort;
	Client(InetAddress host) {
		this.host = host;
	}

	public void setSendPort(int sendPort) {
		this.sendPort = sendPort;
	}

	public void setRecPort(int recPort) {
		this.recPort = recPort;
	}

	public void sendData() throws Exception {
		BufferedReader br;
		DatagramSocket ds;
		DatagramPacket dp;
		String data="";
		System.out.println("Enter the response VOTE_COMMIT || VOTE_ABORT");
		br = new BufferedReader(new InputStreamReader(System.in));
		data = br.readLine();
		System.out.println("Data is "+data);
		ds = new DatagramSocket(sendPort);
		dp = new DatagramPacket(data.getBytes(), data.length(), host, sendPort-1000);
		ds.send(dp);
		ds.close();
	}

	public void recData() throws Exception {
		byte buf[] = new byte[256];
		DatagramPacket dp;
		DatagramSocket ds;

		ds = new DatagramSocket(recPort);
		dp = new DatagramPacket(buf, buf.length);
		ds.receive(dp);
		ds.close();
		String msgStr =  new String(dp.getData(), 0, dp.getLength());
		System.out.println("Client 1 data "+msgStr);
	}
}