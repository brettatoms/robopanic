import java.awt.*;
import java.awt.image.*;
import javax.swing.*;import javax.swing.border.*;

class TileListCellRenderer extends JLabel implements ListCellRenderer{
      
    TileListCellRenderer(){	
		setOpaque(true);  // so we can see the selection highlight		setBorder(new MatteBorder(2, 2, 2, 2, new Color(10, 10, 10)));
    }
    

    public Component getListCellRendererComponent(JList list, Object value,
						 int index, boolean isSelected,
						  boolean cellHasFocus){

	this.setText(((JLabel)value).getText());
	this.setIcon(((JLabel)value).getIcon());
			 			//MatteBorder(int top, int left, int bottom, int right, new Color())	
	if(list == null) return this;
		
	if(isSelected) {
	    setBackground(list.getSelectionBackground());
	    setForeground(list.getSelectionForeground());		setBorder(new MatteBorder(2, 2, 2, 2, list.getSelectionBackground()));
	}
	else {
	    setBackground(list.getBackground());
	    setForeground(list.getForeground());
	}
	setEnabled(list.isEnabled());
	setFont(list.getFont());
	return this;
    }    
}
