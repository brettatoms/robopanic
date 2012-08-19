import java.awt.*;
import java.awt.image.*;
import javax.swing.*;

class TileListModel extends AbstractListModel{
    JLabel[] data;
        	static final int NUM_ROWS = 14;	
	static final int NUM_COLS = 19;
    static final int TILE_WIDTH = 32;
    static final int TILE_HEIGHT = 32;    	int NUM_TILES = NUM_ROWS * NUM_COLS;
    BufferedImage image;	/*
    final static int rows = 14;
    final static int cols = 19;
    final static int tileWidth = 32;
    final static int tileHeight = 32;	*/
    class Point{
		int x, y;
    }
    Point[] tileCoords;

    TileListModel(String imageFile){
	JLabel label = new JLabel();
	Image temp = Toolkit.getDefaultToolkit().getImage(imageFile);
	try {
	    MediaTracker tracker = new MediaTracker(label);
	    tracker.addImage(temp, 0);
	    tracker.waitForID(0);
	}catch ( Exception e ) {}      
	image = new BufferedImage(temp.getWidth(label), temp.getHeight(label),
				  BufferedImage.TYPE_USHORT_565_RGB);
	Graphics2D graphics = image.createGraphics();
	graphics.drawImage(temp, 0, 0, label);	
	//setOpaque(true);  // so we can see the selection highlight
	
	// initialize tile coordinates
	tileCoords = new Point[NUM_TILES];		    	
	for(int i=0; i<NUM_ROWS; i++){
	    for(int j=0; j<NUM_COLS; j++){
		int index = (i*NUM_COLS)+j;
		tileCoords[index] = new Point();
		tileCoords[index].x = ((TILE_WIDTH+1)*j)+1;
		tileCoords[index].y = ((TILE_HEIGHT+1)*i)+1;		    
	    }
	}

	data = new JLabel[NUM_TILES];
	for(int i=0; i<NUM_TILES; i++){
	    data[i] = new JLabel();
	    data[i].setText(Integer.toString(i));
	    int x, y;	    
	    x = tileCoords[i].x;
	    y = tileCoords[i].y;
	    data[i].setIcon(new ImageIcon(image.getSubimage(x, y, TILE_WIDTH, TILE_HEIGHT)));	    
	}
	
	//setOpaque(true);  // so we can see the selection highlight
    }

    public Object getElementAt(int index) { 
	return data[index];
    }
    

    public int getSize(){ return NUM_TILES; };
    
}
