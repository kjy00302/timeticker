package com.kjy00302.timetickerclient;

import com.fazecast.jSerialComm.*;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayList;
import java.util.Arrays;
public class TimeTickerClient {

    public static void main(String[] args) {
        
        SerialPort[] serials = SerialPort.getCommPorts();
        GuiInterface gui = new GuiInterface() {
            private static final long serialVersionUID = -4915800411924994446L;
            final byte[] okResponce = new byte[] {-1, 1};
            SerialPort tickerSerPort;

            String[] getSerialPorts() {
                ArrayList<String> serialPortPaths = new ArrayList<String>();
                for (SerialPort serial: serials) {
                    serialPortPaths.add(serial.getSystemPortName());
                }
                return serialPortPaths.toArray(new String[serialPortPaths.size()]);
            }
            
            boolean serialConnect(int n) {
                tickerSerPort = serials[n];
                tickerSerPort.setBaudRate(115200);
                tickerSerPort.setComPortTimeouts(SerialPort.TIMEOUT_READ_BLOCKING, 100, 0);
                boolean res = tickerSerPort.openPort();
                try {
                    Thread.sleep(4000);
                } catch (InterruptedException e) {
                    // TODO Auto-generated catch block
                    e.printStackTrace();
                }
                if (!res) {
                    return false;
                }
                tickerSerPort.writeBytes(new byte[] {-1, 1}, 2);
                return checkResponce();
            }
            
            byte[] readData() {
                byte[] buffer = new byte[3];
                tickerSerPort.readBytes(buffer, 3);
                byte[] bodyBuffer = new byte[buffer[2]];
                tickerSerPort.readBytes(bodyBuffer, buffer[2]);
                return bodyBuffer;
            }
            
            boolean checkResponce() {
                byte[] buffer = new byte[2];
                tickerSerPort.readBytes(buffer, 2);
                if (Arrays.equals(buffer, okResponce)) {
                    return true;
                }
                return false;
                
            }
            
            byte readFlag() {
                tickerSerPort.writeBytes(new byte[] {-1, 0x03}, 2);
                return readData()[0];
            }
            
            void writeFlag(byte flag) {
                tickerSerPort.writeBytes(new byte[] {-1, 0x02, flag}, 3);
                checkResponce();
            }

            boolean setUserText(String string) {
                short[] textbuffer = new short[string.length()];
                int n=0;
                for (char i:string.toCharArray()) {
                    if (!((i < 256) || (('가' <= i) && (i <= '힣')))) {
                        return false;
                    }
                    textbuffer[n++] = (short) i;
                }
                tickerSerPort.writeBytes(new byte[] {-1, 0x04, (byte) string.length()}, 3);
                byte[] textbytebuffer = new byte[string.length()*2];
                ByteBuffer.wrap(textbytebuffer).order(ByteOrder.LITTLE_ENDIAN).asShortBuffer().put(textbuffer);
                tickerSerPort.writeBytes(textbytebuffer, string.length()*2);
                checkResponce();
                return true;
            }
            
            void setBrightness(byte n) {
                tickerSerPort.writeBytes(new byte[] {-1, 0x22, n}, 3);
                checkResponce();
            }
            
            void timeSync() {
                tickerSerPort.writeBytes(new byte[] {-1, 0x11}, 2);
                checkResponce();
            }
            
            void setTime(byte h, byte m, byte s) {
                tickerSerPort.writeBytes(new byte[] {-1, 0x12, h, m, s}, 5);
                checkResponce();
            }
        };

    }

}
