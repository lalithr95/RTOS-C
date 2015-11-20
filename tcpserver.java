import java.io.*;
import java.net.*;

public class tcpserver {

	public static void main (String args[]) throws Exception
	{
		BufferedReader br;
		InetAddress host;
		host = InetAddress.getLocalHost();
		Server s = new Server(host);

		// client 1
		s.setSendPort(9000);
		s.setRecPort(8001);

		br = new BufferedReader(new InputStreamReader(System.in));
		String str = br.readLine();
		s.sendData();
		s.recData();

		//client 2
		s.setSendPort(9001);
		s.setRecPort(8002);

		br = new BufferedReader(new InputStreamReader(System.in));
		String str1 = br.readLine();
		s.sendData();
		s.recData();

		// send final result to client1
		s.setSendPort(9000);
		s.sendData();

		// send final result to client2
		s.setSendPort(9001);
		s.sendData();
	}

}

class Server {
	InetAddress host;
	int sendPort, recPort;
	int send = 0;
	int count = 0;
	Server(InetAddress host) {
		this.host = host;
	}

	public void setSendPort(int port) {
		this.sendPort = port;
	}

	public void setRecPort(int port) {
		this.recPort = port;
	}

	public void sendData() throws Exception {
		DatagramSocket ds;
		DatagramPacket dp;
		String data = "";

		if(count < 2 && send < 2) {
			data = "VOTE_REQUEST";
		}

		if(count < 2 && send >1) {
			data = "GLOBAL_ABORT";
			data = data + "TRANSACTION ABORTED";
		}

		if(count == 2 && send>1) {
			data = "GLOBAL_COMMIT";
			data = data + "TRANSACTION COMMITED";
		}

		ds = new DatagramSocket(sendPort);
		dp = new DatagramPacket(data.getBytes(), data.length(), host, sendPort-1000);
		ds.send(dp);
		ds.close();
		send++;
	}

	public void recData() throws Exception {
		byte buf[] = new byte[256];
		DatagramPacket dp = null;
		DatagramSocket ds;
		String str = "";
		try {
			ds = new DatagramSocket(recPort);
			dp = new DatagramPacket(buf,buf.length);
			ds.receive(dp);
			ds.close();
		}
		catch(Exception e) {
			e.printStackTrace();
		}
		str = new String(dp.getData(), 0, dp.getLength());
		if(str.equalsIgnoreCase("VOTE_COMMIT")) {
			count++;
		}
	}
}
