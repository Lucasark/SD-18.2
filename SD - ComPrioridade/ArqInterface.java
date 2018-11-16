import java.rmi.Remote;
import java.rmi.RemoteException;

public interface ArqInterface extends Remote {
	void escritaArquivo(String fileName, String input) throws Exception;
	String leituraArquivo(String fileName) throws Exception;
}