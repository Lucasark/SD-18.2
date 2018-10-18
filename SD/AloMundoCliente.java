import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

public class AloMundoCliente {
	private AloMundoCliente() {}
	public static void main(String[] args) {
		String host = (args.length < 1) ? null : args[0];
		try {
				Registry registry = LocateRegistry.getRegistry(host);
				AloMundo stub = (AloMundo) registry.lookup("AloMundo");
				String resposta = stub.digaAloMundo();
				System.out.println("resposta: " + resposta);
			} catch (Exception e) {
				System.err.println("Capturando a exceção no Cliente: " + e.toString());
				e.printStackTrace();
			}
	}
}
