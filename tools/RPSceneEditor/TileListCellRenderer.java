import java.awt.*;
import java.awt.image.*;
import javax.swing.*;

class TileListCellRenderer extends JLabel implements ListCellRenderer{
      
    TileListCellRenderer(){	
		setOpaque(true);  // so we can see the selection highlight
    }
    

    public Component getListCellRendererComponent(JList list, Object value,
						 int index, boolean isSelected,
						  boolean cellHasFocus){

	this.setText(((JLabel)value).getText());
	this.setIcon(((JLabel)value).getIcon());
			 	
	if(list == null) return this;
	
	if(isSelected) {
	    setBackground(list.getSelectionBackground());
	    setForeground(list.getSelectionForeground());
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
