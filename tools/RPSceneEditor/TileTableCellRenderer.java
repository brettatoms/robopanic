import javax.swing.table.*;
import java.awt.*;
import javax.swing.*;
import javax.swing.border.*;

class TileTableCellRenderer extends JLabel implements TableCellRenderer{
    TileTableCellRenderer(){
	super();
	//this.isBordered = true;
	//setOpaque(true);  
	setPreferredSize(new Dimension(32, 32));
	setBorder(null);
    }
    
    Border selectedBorder  = BorderFactory.
	createMatteBorder(1,1,1,1, Color.lightGray);
    Border unselectedBorder = BorderFactory. 
	createMatteBorder(0,0,0,0, Color.white);

    public Component getTableCellRendererComponent(JTable table, 
						   //Object imageIcon, 
						   Object value, 
						   boolean isSelected, 
						   boolean hasFocus, 
						   int row, int column){

	if(value != null) {
	    //String text = 
	    //if(((JLabel)value).getText().equals("-1"))
	    //setIcon(null);
	    setIcon(((JLabel)value).getIcon());
	}
	if (isSelected) setBorder(selectedBorder);
	else setBorder(unselectedBorder);
    
	return this;
    }
}
