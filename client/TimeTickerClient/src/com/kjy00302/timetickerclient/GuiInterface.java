package com.kjy00302.timetickerclient;

import java.awt.BorderLayout;
import java.awt.GridLayout;

import javax.swing.*;

abstract public class GuiInterface extends JFrame {
    private static final long serialVersionUID = 9005526866811610322L;
    
    GuiInterface(){
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLayout(new BorderLayout());
        setSize(400,300);
        setTitle("시계 설정");
        
        JLabel resultLabel = new JLabel("연결되지 않았습니다");
        add(resultLabel, BorderLayout.SOUTH);
        
        JPanel configPanel = new JPanel();
        configPanel.setLayout(new GridLayout(3,1));
        configPanel.setEnabled(false);        
        
        JPanel flagSetPanel = new JPanel();
        JLabel flagSetLabel = new JLabel("설정:");
        flagSetPanel.add(flagSetLabel);
        JCheckBox cb7 = new JCheckBox("24H");
        flagSetPanel.add(cb7);
        JCheckBox cb6 = new JCheckBox("한글로 출력");
        flagSetPanel.add(cb6);
        JCheckBox cb5 = new JCheckBox("자동시간동기화");
        flagSetPanel.add(cb5);
        JCheckBox cb4 = new JCheckBox("자동밝기");
        flagSetPanel.add(cb4);
        JButton confFlagReadButton = new JButton("읽기");
        confFlagReadButton.setEnabled(false);
        confFlagReadButton.addActionListener(e -> {
            byte flag = readFlag();
            cb7.setSelected((flag & 0b10000000) != 0);
            cb6.setSelected((flag & 0b01000000) != 0);
            cb5.setSelected((flag & 0b00100000) != 0);
            cb4.setSelected((flag & 0b00010000) != 0);
            resultLabel.setText("설정을 읽었습니다");
        });
        flagSetPanel.add(confFlagReadButton);
        JButton confFlagWriteButton = new JButton("쓰기");
        confFlagWriteButton.setEnabled(false);
        confFlagWriteButton.addActionListener(e -> {
            byte flag = 0;
            flag |= cb7.isSelected() ? 0b10000000 : 0;
            flag |= cb6.isSelected() ? 0b01000000 : 0;
            flag |= cb5.isSelected() ? 0b00100000 : 0;
            flag |= cb4.isSelected() ? 0b00010000 : 0;
            flag |= 3;
            writeFlag(flag);
            resultLabel.setText("설정을 썼습니다");
        });
        flagSetPanel.add(confFlagWriteButton);
        configPanel.add(flagSetPanel);
        
        JPanel userTextPanel = new JPanel();
        JLabel userTextLabel = new JLabel("사용자 문구:");
        userTextPanel.add(userTextLabel);
        JTextField userTextBox = new JTextField();
        userTextBox.setColumns(32);
        userTextPanel.add(userTextBox);
        JButton setUserTextButton = new JButton("설정");
        setUserTextButton.setEnabled(false);
        setUserTextButton.addActionListener(e -> {
            if (userTextBox.getText().length() <= 32) {
                if (!setUserText(userTextBox.getText())) {
                    resultLabel.setText("지원하지 않는 글자가 있습니다");
                } else {
                    resultLabel.setText("문구를 설정했습니다");
                }
            } else {
                resultLabel.setText("길이가 32자를 넘습니다");
            }
        });
        userTextPanel.add(setUserTextButton);
        configPanel.add(userTextPanel);
        

        JPanel otherPanel = new JPanel();
        JLabel brightnessLabel = new JLabel("밝기:");
        otherPanel.add(brightnessLabel);
        JComboBox<String> brightnessComboBox = new JComboBox<String>(
                new String[] {"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16"});
        otherPanel.add(brightnessComboBox);
        JButton setBrightnessButton = new JButton("설정");
        setBrightnessButton.setEnabled(false);
        setBrightnessButton.addActionListener(e -> {
            setBrightness((byte) brightnessComboBox.getSelectedIndex());
            resultLabel.setText("밝기를 설정했습니다");
        });
        otherPanel.add(setBrightnessButton);
        JLabel timeSetLabel = new JLabel("시간:");
        otherPanel.add(timeSetLabel);
        JSpinner hourSpinner = new JSpinner(new SpinnerNumberModel(0, 0, 23, 1));
        otherPanel.add(hourSpinner);
        JSpinner minuteSpinner = new JSpinner(new SpinnerNumberModel(0, 0, 59, 1));
        otherPanel.add(minuteSpinner);
        JSpinner secondSpinner = new JSpinner(new SpinnerNumberModel(0, 0, 59, 1));
        otherPanel.add(secondSpinner);
        JButton timeSetButton = new JButton("설정");
        timeSetButton.setEnabled(false);
        timeSetButton.addActionListener(e -> {
            setTime(
                    (byte) (int) hourSpinner.getValue(),
                    (byte) (int) minuteSpinner.getValue(),
                    (byte) (int) secondSpinner.getValue()
                    );
            resultLabel.setText("시간을 설정했습니다");
        });
        otherPanel.add(timeSetButton);
        
        JLabel timeSyncLabel = new JLabel("시간 자동 조정:");
        otherPanel.add(timeSyncLabel);
        JButton timeSyncButton = new JButton("동기화");
        timeSyncButton.setEnabled(false);
        timeSyncButton.addActionListener(e -> {
            timeSync();
            resultLabel.setText("시간 동기화를 요청했습니다");
        });
        otherPanel.add(timeSyncButton);
        configPanel.add(otherPanel);
        
        add(configPanel);
        
        JPanel serialPortPanel = new JPanel();
        String[] serialPorts = getSerialPorts();
        JComboBox<String> comboBox = new JComboBox<String>(serialPorts);
        serialPortPanel.add(comboBox);
        JButton connectButton = new JButton("Connect");
        connectButton.addActionListener(e -> {
            if (serialConnect(comboBox.getSelectedIndex())) {
                resultLabel.setText("연결되었습니다");
                connectButton.setEnabled(false);
                configPanel.setEnabled(true);
                confFlagReadButton.setEnabled(true);
                confFlagWriteButton.setEnabled(true);
                setUserTextButton.setEnabled(true);
                setBrightnessButton.setEnabled(true);
                timeSetButton.setEnabled(true);
                timeSyncButton.setEnabled(true);
            }
        });
        serialPortPanel.add(connectButton);
        add(serialPortPanel, BorderLayout.NORTH);
        pack();
        setVisible(true);
    }

    abstract String[] getSerialPorts();
    abstract boolean serialConnect(int n);
    abstract byte readFlag();
    abstract void writeFlag(byte flag);
    abstract boolean setUserText(String string);
    abstract void setBrightness(byte n);
    abstract void timeSync();
    abstract void setTime(byte h, byte m, byte s);
    

}
