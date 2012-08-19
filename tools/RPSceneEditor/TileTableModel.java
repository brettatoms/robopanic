/* TileTableModel.java
 *
 *
 */

import javax.swing.*;
import javax.swing.table.*;

public class TileTableModel extends AbstractTableModel{

    final static int numCols = 20; // 640 / 32
    final static int numRows = 15; // 480 / 32
    TileData[][] tileData;
    	public int cols(){ return numCols; }
	public int rows(){ return numRows; }	
    class TileData{
	TileData(){
	    //label = new JLabel(new ImageIcon("blank.jpg")); 
	    label = new JLabel(); 
	    type = -1;	    
	}	
	JLabel label;
	int type;
    }

    TileTableModel(){	
		tileData = new TileData[numRows][];
		for(int i=0; i<numRows; i++){
			tileData[i] = new TileData[numCols];
			for(int j=0; j<numCols; j++)
				tileData[i][j] = new TileData();
		}
    }
    
    public String getColumnName(int col) { 
		return null;
    }

    public int getRowCount() { return numRows; }

    public int getColumnCount() { return numCols; }

    public Class getColumnClass(int c) { 
		return getValueAt(0,c).getClass(); 
    }

    public Object getValueAt(int row, int col) { 
		return tileData[row][col].label;	
    }

    public boolean isCellEditable(int row, int col){ return false; }

    public void setValueAt(Object value, int row, int col) {	
		if(value == null) System.out.println("  value == NULL");	
		tileData[row][col].type = Integer.parseInt(((JLabel)value).getText());	
		if(tileData[row][col].type == -1)  tileData[row][col].label.setIcon(null);	
		tileData[row][col].label.setIcon(((JLabel)value).getIcon());
		fireTableCellUpdated(row, col);
    }
}
