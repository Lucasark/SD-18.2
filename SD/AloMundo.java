import java.rmi.Remote;
import java.rmi.RemoteException;

public interface AloMundo extends Remote {
	String digaAloMundo() throws RemoteException;
}