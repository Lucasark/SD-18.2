import java.rmi.Remote;
import java.rmi.RemoteException;

public interface Interface extends Remote {
	String lerArquivo() throws Exception;
	String escreverArquivo() throws Exception;
}