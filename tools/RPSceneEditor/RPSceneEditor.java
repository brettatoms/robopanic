/* RPSceneEditor.java
 *
 * Written By: brett adams, adams@cs.unt.edu
 * Description: create scene data files for the RoboPanic game
 *
 */

import java.io.*;
import java.awt.*;
import java.awt.image.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.table.*;
import java.util.*;
import TileTableCellRenderer;
import TileTableModel;import TileListCellRenderer;


public class RPSceneEditor extends JFrame implements ActionListener, MouseListener{
    	
    static final int NUM_TABLE_ROWS = 15;
    static final int NUM_TABLE_COLS = 20;
    static final int TILE_WIDTH = 32;
    static final int TILE_HEIGHT = 32;    	int currentRowMin, currentRowMax, currentColMin, currentColMax;
    JList tileList;    
    final JTable tileMapTable  = new JTable();
    JPanel mapPanel;
    TileAttribute[][] tileAttributes;
    JCheckBox walkBox = null;
    JCheckBox overBox = null;
    JCheckBox switchBox = null;
    String tilesFile = "tiles.jpg";

    ////////// constructor //////////
    public RPSceneEditor() {
		super("RoboPanic Scene Editor");
		setSize(785, 565);
		addWindowListener(new WindowAdapter() {
			public void windowClosing(WindowEvent e) {System.exit(0);}
			public void windowOpened(WindowEvent e) {}		});				currentRowMin = currentRowMax = currentColMin = currentColMax = -1;
		
		tileAttributes = new TileAttribute[NUM_TABLE_ROWS][NUM_TABLE_COLS];
		for(int i=0; i<NUM_TABLE_ROWS; i++){
			for(int j=0; j<NUM_TABLE_COLS; j++){
				tileAttributes[i][j] = new TileAttribute();
				tileAttributes[i][j].row = i;				tileAttributes[i][j].col = j;
			}
		}
		
		initGUI();
    }

	/////////////////////////////////////////////////////////////
		static final int ENGNAME_LENGTH = 16;	static final int SCENENAME_LENGTH = 16;
    static final int DESCRIPTION_LENGTH = 128;
    static final int TILESET_LENGTH = 32;		/////////////////////////////////////////////////////////////	
    static class SceneFileHdr{
		//public static StringBuffer engName = new StringBuffer(ENGNAME_LENGTH);
		public static String engName = new String();
		public final static int engVersion = 70;		public static String sceneName = new String();
		//public static StringBuffer mapDesc = new StringBuffer(DESCRIPTION_LENGTH);
		public static String sceneDesc = new String();
		//public static StringBuffer tileSet = new StringBuffer(TILESET_LENGTH);
		public static String tileSet = new String();
		public final static int tileRows = NUM_TABLE_ROWS;
		public final static int tileCols = NUM_TABLE_COLS;
    }
	/////////////////////////////////////////////////////////////	
    static class TileAttribute{
		public TileAttribute(){ 
			properties = new BitSet(NUM_PROPS);
			tileType = row = col = -1;			//properties.set(WALKTHRUABLE);
		}
		public BitSet properties;
		public int tileType;
		public int row, col;	
		final static int WALKTHRUABLE = 0;
		final static int SWITCH = 1;		final static int OVER = 2;
		//final static int MOVEABLE = 2;
		final static int NUM_PROPS = 3;
    }

    //////////////////////////////////////////////////

    void saveDataFile(File file){		
		DataOutputStream outStream;
		SceneFileHdr.engName = new String("RoboPanic");		SceneFileHdr.sceneName = new String("sceneName");
		SceneFileHdr.sceneDesc = new String("this is the description");		
		SceneFileHdr.tileSet = new String("tiles.bmp");
	
		try{
			outStream =new DataOutputStream(new BufferedOutputStream(new FileOutputStream(file)));
			byte[] data;
	    
			// write engine name
			data = new byte[ENGNAME_LENGTH];
			copyBytes(data, SceneFileHdr.engName.getBytes());
			outStream.write(data);

			//write engine version
			outStream.writeInt(SceneFileHdr.engVersion);
			//write map name
			data = new byte[SCENENAME_LENGTH];
			copyBytes(data, SceneFileHdr.sceneName.getBytes());
			outStream.write(data);		
			// write map description
			data = new byte[DESCRIPTION_LENGTH];
			copyBytes(data, SceneFileHdr.sceneDesc.getBytes());
			outStream.write(data);

			// write tileset
			data = new byte[TILESET_LENGTH];
			copyBytes(data, SceneFileHdr.tileSet.getBytes());
			outStream.write(data);

			// write num rows and cols
			outStream.writeInt(SceneFileHdr.tileRows);
			outStream.writeInt(SceneFileHdr.tileCols);	    
	    	    
			// write each tiles data
			for(int i=0; i<SceneFileHdr.tileRows; i++)
				for(int j=0; j<SceneFileHdr.tileCols; j++){		    
					outStream.writeInt(tileAttributes[i][j].tileType);
					for(int n=0; n<tileAttributes[i][j].NUM_PROPS; n++)
						outStream.write(tileAttributes[i][j].properties.get(n) ? (byte)1:(byte)0);
				}

			outStream.flush();
			outStream.close();
		}catch(IOException e){System.out.println(e);}
    }
	//////////////////////////////////////////////////	
    void openDataFile(File file){
		//BufferedInputStream inStream;	
		TileAttribute[][] tileData;
		tileData = new TileAttribute[NUM_TABLE_ROWS][NUM_TABLE_COLS];
					
		for(int i=0; i<NUM_TABLE_ROWS; i++){	    
			for(int j=0; j<NUM_TABLE_COLS; j++){
				tileData[i][j] = new TileAttribute();
				tileData[i][j].row = i;
				tileData[i][j].col = j;		
			}	    
		}	

		DataInputStream inStream;	
		try{
			inStream = new DataInputStream(new BufferedInputStream(new FileInputStream(file)));
			int offset = 0;
			byte[] buff;

		    buff = new byte[ENGNAME_LENGTH];
		    //inStream.read(buff, offset, buff.length);
			inStream.read(buff);
			SceneFileHdr.engName = new String(buff);
			System.out.println("engName: " + SceneFileHdr.engName);
	    
			// read engVersion (4 bytes)
			//SceneFileHdr.engVersion = inStream.readInt();
			float version = inStream.readInt();			if(version != SceneFileHdr.engVersion){				JOptionPane.showMessageDialog(this, "Invalid .rpd version: " + version/100f											  + "\nThis version of RPSceneEditor can only "											  + "edit and save v0.7 .rpd files");
				//JDialog d = new JDialog(this, "Invalid file version!");
				//d.show();				System.out.println("version = " + version/100);
								return;
			}				
			System.out.println("version: " + SceneFileHdr.engVersion);
			// read scene name
			buff = new byte[SCENENAME_LENGTH];			inStream.read(buff);			SceneFileHdr.sceneName = new String(buff);			System.out.println("name: " + SceneFileHdr.sceneName);
					
			// read scene description
			buff = new byte[DESCRIPTION_LENGTH];	    
			inStream.read(buff);
			SceneFileHdr.sceneDesc = new String(buff);
			System.out.println("description: " + SceneFileHdr.sceneDesc);
	    	
    	    // read tileset name
			buff = new byte[TILESET_LENGTH];
			inStream.read(buff);
			SceneFileHdr.tileSet = new String(buff);
			System.out.println("tile set: " + SceneFileHdr.tileSet);

			// read nrow and ncols
			//SceneFileHdr.tileRows = inStream.readInt();
			//SceneFileHdr.tileCols = inStream.readInt();			inStream.readInt();
			inStream.readInt();
			System.out.println(SceneFileHdr.tileRows + " x " + 
			       SceneFileHdr.tileCols);
	    	    
			// read each tiles data
			for(int i=0; i<SceneFileHdr.tileRows; i++){		
				for(int j=0; j<SceneFileHdr.tileCols; j++){
				    tileData[i][j].tileType = inStream.readInt();					//tileAttributes[i][j].tileType = inStream.readInt();
					//fSystem.out.print(" " + tileData[i][j].tileType);
					for(int n=0; n<tileData[i][j].NUM_PROPS; n++){					//for(int n=0; n<tileAttributes[i][j].NUM_PROPS; n++){
						byte bit = inStream.readByte();
						if(bit == 1) tileData[i][j].properties.set(n);							//tileAttributes[i][j].properties.set(n);
						else if(bit == 0)  tileData[i][j].properties.clear(n);
							//tileAttributes[i][j].properties.clear(n);						
						else {
							System.out.println("unknown bit value: " + bit);
							System.exit(1);
						}
					}		    
				}
				System.out.println();
			}
	    
	    //SceneSceneFileHdr.engName
	}catch(IOException e){
	    System.out.println(e);	    
	}
	
	//set the tiles and attributes from data read in
	//setTiles(tileData);
	//setAttributeBoxes();	//setTiles(tileAttributes);	setTiles(tileData);
    }

	/////////////////////////////////////////////////////////////	
    void setTiles(TileAttribute[][] data){
		TileListModel model = (TileListModel)(tileList.getModel());
		for(int i=0; i<SceneFileHdr.tileRows; i++){
			for(int j=0; j<SceneFileHdr.tileCols; j++){
				// set table value
				int index = ((i*SceneFileHdr.tileCols)+j);								if(data[i][j].tileType != -1){
					JLabel listLabel = (JLabel)(model.getElementAt(data[i][j].tileType));
					tileMapTable.setValueAt(listLabel, i, j);
				}else tileMapTable.setValueAt(new JLabel("-1"), i, j);
				tileAttributes[i][j].tileType = data[i][j].tileType;
				// set attributes				tileAttributes[i][j].properties = new BitSet(tileAttributes[i][j].NUM_PROPS);					
				for(int n=0; n<tileAttributes[i][j].NUM_PROPS; n++)					if(data[i][j].properties.get(n))
						tileAttributes[i][j].properties.set(n);					   							 								
			}
		}
		setAttributeBoxes();		
    }    
	//////////////////////////////////////////////////////////////	void resetForNewFile(){		
		for(int i=0; i<NUM_TABLE_ROWS; i++){			for(int j=0; j<NUM_TABLE_COLS; j++){			
				tileMapTable.setValueAt(new JLabel("-1"), i, j);							}		}								SceneFileHdr.engName = new String("RoboPanic");		SceneFileHdr.sceneName = new String("sceneName");
		SceneFileHdr.sceneDesc = new String("this is the description");		
		SceneFileHdr.tileSet = new String("tiles.bmp");
		tileAttributes = new TileAttribute[NUM_TABLE_ROWS][NUM_TABLE_COLS];
		for(int i=0; i<NUM_TABLE_ROWS; i++){
			for(int j=0; j<NUM_TABLE_COLS; j++){
				tileAttributes[i][j] = new TileAttribute();
				tileAttributes[i][j].row = i;				tileAttributes[i][j].col = j;
			}
		}	}
	
    //////////////////////////////////////////////////////////////
    JFileChooser fileChooser = new JFileChooser(new String("./"));
    void initMenuBar(){

		// menu bar
		JMenuBar menuBar = new JMenuBar();
		JMenu menu = new JMenu("File");
		JMenuItem item;	
		item = new JMenuItem("New");
		item.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent e){				
					resetForNewFile();
			}
	    });
		menu.add(item);		
		item = new JMenuItem("Save");
		item.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent e){
				int val = fileChooser.showSaveDialog(fileChooser);
				if(val == JFileChooser.APPROVE_OPTION)
					saveDataFile(fileChooser.getSelectedFile());
			}
	    });
		menu.add(item);

		item = new JMenuItem("Open");
		item.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent e){		    				
				int val = fileChooser.showOpenDialog(fileChooser);
				if(val == JFileChooser.APPROVE_OPTION)
					openDataFile(fileChooser.getSelectedFile());
			}
	    });
		menu.add(item);	

		item = new JMenuItem("Exit");
		item.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent e){
				System.out.println("quitting RPSceneEditor...");
				System.exit(1);
			}
	    });
		menu.add(item);
		
		menuBar.add(menu);	
		getContentPane().add(menuBar, "North");
    }

    //////////////////////////////////////////////////////////////

    void initAttributeBoxes(){
		// attributes
		JPanel panel = new JPanel();
		panel.setLayout(new BoxLayout(panel, BoxLayout.Y_AXIS));
	
		walkBox = new JCheckBox("Walk Through", false);				walkBox.addActionListener(new ActionListener(){					public void actionPerformed(ActionEvent e){
				if(walkBox.isSelected()){					for(int i=currentRowMin; i<=currentRowMax; i++){						for(int j=currentColMin; j<=currentColMax; j++){							tileAttributes[i][j].properties.set(TileAttribute.WALKTHRUABLE);													}					}				}else{					for(int i=currentRowMin; i<=currentRowMax; i++){						for(int j=currentColMin; j<=currentColMax; j++){							tileAttributes[i][j].properties.clear(TileAttribute.WALKTHRUABLE);													}					}				}					
			}
	    });
		panel.add(walkBox);

		overBox = new JCheckBox("Draw Over Character", false);		
		overBox.addActionListener(new ActionListener(){					public void actionPerformed(ActionEvent e){
				if(overBox.isSelected()){					for(int i=currentRowMin; i<=currentRowMax; i++){						for(int j=currentColMin; j<=currentColMax; j++){							tileAttributes[i][j].
								properties.set(TileAttribute.OVER);													}					}				}else{					for(int i=currentRowMin; i<=currentRowMax; i++){						for(int j=currentColMin; j<=currentColMax; j++){							tileAttributes[i][j].
								properties.clear(TileAttribute.OVER);													}					}				}			}
	    });
		panel.add(overBox);
		
		switchBox = new JCheckBox("Switch", false);		switchBox.addActionListener(new ActionListener(){					public void actionPerformed(ActionEvent e){		
				if(switchBox.isSelected()){					for(int i=currentRowMin; i<=currentRowMax; i++){						for(int j=currentColMin; j<=currentColMax; j++){							tileAttributes[i][j].
								properties.set(TileAttribute.SWITCH);													}					}				}else{					for(int i=currentRowMin; i<=currentRowMax; i++){						for(int j=currentColMin; j<=currentColMax; j++){							tileAttributes[i][j].
								properties.clear(TileAttribute.SWITCH);													}					}				}					
			}
	    });
		panel.add(switchBox);

		getContentPane().add(panel, "South");
	
    }    

    ////////////////////////////////////////////////////////

    // setup tile map table 
    void initTileMapTable(){		
		
		tileMapTable.setModel(new TileTableModel());	
		tileMapTable.setIntercellSpacing(new Dimension(0,0));
		tileMapTable.setShowGrid(false);
		tileMapTable.setRowHeight(TILE_HEIGHT);
		tileMapTable.setTableHeader(null);  // no header
		tileMapTable.addMouseListener(this);  
		TableColumn column;	
		for(int i=0; i<NUM_TABLE_COLS; i++){
			column = tileMapTable.getColumnModel().getColumn(i);
			column.setPreferredWidth(TILE_WIDTH);
			column.setCellRenderer(new TileTableCellRenderer());
			column.setMaxWidth(TILE_WIDTH);
			//column.setMinWidth(TILE_WIDTH);	    
		}	
					
		tileMapTable.setDefaultRenderer(JLabel.class, 
					new TileTableCellRenderer());	
		// setup table selection model				tileMapTable.setSelectionMode(ListSelectionModel.SINGLE_INTERVAL_SELECTION);
	
		///////// row selection	/////////
		ListSelectionModel rowSM = tileMapTable.getSelectionModel();
		rowSM.addListSelectionListener(new ListSelectionListener()	
			{
                public void valueChanged(ListSelectionEvent e) {
                    ListSelectionModel lsm = (ListSelectionModel)e.getSource();										currentRowMin = lsm.getMinSelectionIndex();
					currentRowMax = lsm.getMaxSelectionIndex();															if(currentRowMin != -1 && currentColMin != -1)					
						setAttributeBoxes();
				}
			});

		////////// column selection /////////
		tileMapTable.setColumnSelectionAllowed(true);	
		ListSelectionModel colSM = tileMapTable.getColumnModel().getSelectionModel();
		colSM.addListSelectionListener(new ListSelectionListener()
			{
                public void valueChanged(ListSelectionEvent e) {
                    ListSelectionModel lsm = (ListSelectionModel)e.getSource();										currentColMin = lsm.getMinSelectionIndex();
					currentColMax = lsm.getMaxSelectionIndex();										if(currentRowMin != -1 && currentColMin != -1)
						setAttributeBoxes();
				}
	    });	

	
		JScrollPane scrollPane = new JScrollPane(tileMapTable);	
		tileMapTable.addRowSelectionInterval(0,0);
		tileMapTable.addColumnSelectionInterval(0,0);
		getContentPane().add(scrollPane, BorderLayout.CENTER);
	}

    ////////////////////////////////////////////////////////////////

    void setAttributeBoxes(){				boolean  walkValue, switchValue, overValue;				walkValue = switchValue = overValue = true;
				for(int i=currentRowMin; i<=currentRowMax; i++){			for(int j=currentColMin; j<=currentColMax; j++){
				walkValue &= tileAttributes[i][j].properties.get(TileAttribute.WALKTHRUABLE);
				switchValue &= tileAttributes[i][j].properties.get(TileAttribute.SWITCH);
				overValue &= tileAttributes[i][j].properties.get(TileAttribute.OVER);			}		}		
		if(walkBox != null) walkBox.setSelected(walkValue);
		if(switchBox != null) switchBox.setSelected(switchValue);		if(overBox != null) overBox.setSelected(overValue);						//System.out.println("leaving setAttributeBoxes()");
    }

    //////////////////////////////////////////////////////////////

    public void initGUI(){
		getContentPane().setLayout(new BorderLayout());	
		initMenuBar();
		initAttributeBoxes();
		initTileMapTable();
	
		// tile list selector		
		TileListCellRenderer tileCells = new TileListCellRenderer();
		tileList = new JList(new TileListModel(tilesFile));
		tileList.setCellRenderer(tileCells);
		tileList.setSelectedIndex(0);

		JScrollPane scrollPane = new JScrollPane(tileList);
		getContentPane().add(scrollPane, "East");
		System.out.println("leaving RPSceneEditor.initGUI()");
    }
    
    ///////////////////////////////////////////////////////////

    public void actionPerformed(ActionEvent event){
		System.out.println("action");
    }

    //////////////////////////////////////////////////////////

    public void mouseEntered(MouseEvent e){}
    public void mouseExited(MouseEvent e){}
    public void mousePressed(MouseEvent e){}
    public void mouseReleased(MouseEvent e){}

    public void mouseClicked(MouseEvent e){
		if(e.getSource().equals(tileMapTable)){	    
			if((e.getModifiers() & MouseEvent.BUTTON3_MASK) == MouseEvent.BUTTON3_MASK){				
				JLabel listLabel = (JLabel)(tileList.getSelectedValue());				for(int i=currentRowMin; i<=currentRowMax; i++){					for(int j=currentColMin; j<=currentColMax; j++){
						tileMapTable.setValueAt(listLabel, i, j);
						tileAttributes[i][j].tileType = 							Integer.parseInt(listLabel.getText());							}				}				
			}
		}
    }

    ///////////////////////////////////////////////////////////

    public static void main(String[] args) {
		RPSceneEditor editor = new RPSceneEditor();
		editor.setVisible(true);
    }

    //////////////////////////////////////////////////////////

    public void initTextArea(){

	// drawing area
	/*
	  mapPanel = new JPanel();
	  mapPanel.setLayout(new BorderLayout());	
	mapPanel.addMouseListener(this);	
	Canvas canvas = new Canvas();
	mapPanel.add(canvas);
	getContentPane().add(mapPanel, "Center");
	*/
	/*
 	JPanel leftP = new JPanel(); // hold labels
	JPanel rightP = new JPanel(); // hold text fields	
	

 	leftP.setLayout(new BorderLayout());
	leftP.setPreferredSize(new Dimension(200, 200));
 	rightP.setLayout(new BorderLayout());
	
	JPanel rowP = new JPanel();
	JTextArea textArea;
	JTextField textField;
	JLabel label;	
	int pos = 0;

	label = new JLabel("Map Description");
	label.setHorizontalAlignment(JLabel.LEFT);
	rowP.add(label);

	ImageIcon image = new ImageIcon("tiles.bmp");
	label.setIcon(image);

	textArea = new JTextArea(3, 30);
	rowP.add(textArea);
	getContentPane().add(rowP, "North");	

	rowP = new JPanel();
	label = new JLabel("Tile Set");
	label.setIcon(image);
	rowP.add(label);
	textField = new JTextField();
	rowP.add(textField);
	getContentPane().add(rowP, "North");	
	*/
	/*
	JTextArea textArea;
	JTextField textField;
	JLabel label;
	label = new JLabel("Map Description");
	leftP.add(label);
	
	textArea = new JTextArea(3, 1);
	rightP.add(textArea);

	label = new JLabel("Tile Set");
	leftP.add(label);
	textField = new JTextField();
	rightP.add(textField);

	label = new JLabel("Tiles/Column");
	leftP.add(label);
	textField = new JTextField();
	rightP.add(textField);

	label = new JLabel("Tiles/Row");
	leftP.add(label);
	textField = new JTextField();
	rightP.add(textField);
	*/
    }    

    int copyBytes(byte[] dest, byte[] src){
		if(dest.length > src.length){
			for(int i=0; i<src.length; i++)
				dest[i] = src[i];
			Arrays.fill(dest, src.length, dest.length-1, (byte)0);
			return src.length;
		}
		else if(dest.length < src.length){
			for(int i=0; i<dest.length; i++)
				dest[i] = src[i];
			Arrays.fill(src, dest.length, src.length-1, (byte)0);
			return dest.length;
		}
		return 0; // should never get here
    }

} // end RPSceneEditor class
