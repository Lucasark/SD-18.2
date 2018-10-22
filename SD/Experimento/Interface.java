import java.rmi.Remote;
import java.rmi.RemoteException;

public interface Interface extends Remote {
	String lerArquivo(String nomeArq) throws Exception;
	String escreverArquivo(String nomeArq,String input) throws Exception;
}