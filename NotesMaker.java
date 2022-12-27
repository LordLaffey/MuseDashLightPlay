/*
 * unfinished
 */
package src;

import com.formdev.flatlaf.*;
import javax.swing.*;
import javax.swing.event.*;

import java.awt.*;
import java.awt.event.*;
import java.io.*;

public class NotesMaker extends JFrame{

	final static Font commonFont = new Font("JetBrains Mono", Font.BOLD, 12);
	
	static File file = new File("woc.rbq");
	
	static int size = 0;
	
	static File mainFile = null;
	static int cntFile = 0;

	static int cnt = 0;

	static boolean upStates[] = new boolean[3010];
	static boolean downStates[] = new boolean[3010];

	public class RBQFile {
	
		int type;
		int size;
		boolean upStates[] = new boolean[3010];
		boolean downStates[] = new boolean[3010];
	}

	App() {
		
		size = 1000;

		try {
			UIManager.setLookAndFeel(new FlatDarculaLaf());
		} catch (Exception e) {
			e.printStackTrace();
		}

		JPanel mainPanel = new JPanel();
		JPanel upPanel = new JPanel();
		JPanel downPanel = new JPanel();
		JScrollPane scrollPane = new JScrollPane(mainPanel);
		JMenuBar menuBar = new JMenuBar();
		JMenu menu = new JMenu("FILE");
		JMenuItem saveItem = new JMenuItem("SAVE");
		JMenuItem openItem = new JMenuItem("OPEN");
		JMenuItem newItem = new JMenuItem("NEW");

		mainPanel.setLayout(new GridLayout(2,1,0,0));
		mainPanel.add(upPanel);
		mainPanel.add(downPanel);
		menuBar.add(menu);
		menu.add(newItem);
		menu.add(openItem);
		menu.add(saveItem);

		class MyChangeListener implements ChangeListener {
			@Override
			public void stateChanged(ChangeEvent e) {
				String title = getTitle();
				int pos=0,sum=0;
				while(title.charAt(pos)>='0'&&title.charAt(pos)<='9')
				{
					sum=sum*10+title.charAt(pos)-'0';
					pos++;
				}
				System.out.print(pos + " " + title + "\n");
				if (title.charAt(title.length()-1)=='u') {
					upStates[pos] = !upStates[pos];
					if (upStates[pos]) {
						cnt++;
					} else {
						cnt--;
					}
				} else {
					downStates[pos] = !downStates[pos];
					if (downStates[pos]) {
						cnt++;
					} else {
						cnt--;
					}
				}
			}
		}

		for (int i = 0; i < size; i++) {
			JCheckBox upCheckBox = new JCheckBox(Integer.toString(i)+"u");
			upCheckBox.addChangeListener(new MyChangeListener());
			upCheckBox.setFont(commonFont);

			JCheckBox downCheckBox = new JCheckBox(Integer.toString(i)+"d");
			downCheckBox.setFont(commonFont);
			downCheckBox.addChangeListener(new MyChangeListener());
			
			upPanel.add(upCheckBox);
			downPanel.add(downCheckBox);
		}

		class SaveListener implements ActionListener {
			@Override
			public void actionPerformed(ActionEvent e) {
				try {
					saveNotes();
				} catch (IOException exception) {
					exception.printStackTrace();
				}
			}
		}
		
		class OpenListener implements ActionListener {
			@Override
			public void actionPerformed(ActionEvent e) {
				try {
					openNotes();
				} catch (IOException exception) {
					exception.printStackTrace();
				}
			}
		}
		
		class NewListener implements ActionListener {
			@Override
			public void actionPerformed(ActionEvent e) {
				try {
					newNotes();
				} catch (IOException exception) {
					exception.printStackTrace();
				}
			}
		}

		newItem.addActionListener(new NewListener());
		saveItem.addActionListener(new SaveListener());
		openItem.addActionListener(new OpenListener());

		add(scrollPane, BorderLayout.CENTER);
		setJMenuBar(menuBar);

		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setSize(100, 100);
		setVisible(true);
	}

	public static void newNotes() throws IOException {
		mainFile = new File("Untitled" + Integer.toString(++cntFile) + ".rbq");
		mainFile.createNewFile();
	}

	public static void saveNotes() throws IOException {
		FileWriter writer = new FileWriter(mainFile);
		int cnt = 0;
		writer.write(Integer.toString(cnt));
		for (int i = 0; i < size; ++i) {
			if (upStates[i] == true) {
				writer.write(Integer.toString(cnt));
				writer.write(" ");
				writer.write("1");
			}
			if (downStates[i] == true) {
				writer.write(Integer.toString(cnt));
				writer.write(" ");
				writer.write("2");
			}
		}
		writer.flush();
		writer.close();
	}
	
	public static void openNotes() throws IOException {
		// FileReader reader = new FileReader(mainFile);
	}
	
    public static void main(String[] args) throws Exception {
		
		new App();
		
    }
}
