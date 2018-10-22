import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.Callable;

public class ThreadLeitura implements Callable<String>{

  //public BlockingQueue<String> blockingQueue = null;
  public String nomeArq = "";

  public ThreadLeitura(BlockingQueue<String> blockingQueue,String nomeArq){
    //this.blockingQueue = blockingQueue;     
    this.nomeArq = nomeArq;
  }

  public ThreadLeitura(String nomeArq){   
    this.nomeArq = nomeArq;
  }

  @Override
  public String call() throws Exception{
    String texto = "";

    BufferedReader br = null;
     try {
            br = new BufferedReader(new FileReader(new File(nomeArq)));
            String linha =null;
            while((linha=br.readLine())!=null){
                texto = texto + "\n" + linha; 
                //blockingQueue.put(buffer);
            }
            //blockingQueue.put("EOF");  //When end of file has been reached

        } catch (FileNotFoundException e) {

            e.printStackTrace();
        } catch (IOException e) {

            e.printStackTrace();
        } finally{
            try {
                br.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        return texto;
  }



}