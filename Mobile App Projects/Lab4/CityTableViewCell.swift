

import UIKit

class CityTableViewCell: UITableViewCell {

    
    @IBOutlet weak var cityLbl: UILabel!
    
    @IBOutlet weak var couLbl: UILabel!
    
 
    @IBOutlet weak var imageLbl: UIImageView!
    {
       didSet
       {
        imageLbl.layer.cornerRadius = imageLbl.bounds.width / 2
        imageLbl.clipsToBounds = true
        }
    }
    
    override func awakeFromNib() {
        super.awakeFromNib()
        // Initialization code
    }

    override func setSelected(_ selected: Bool, animated: Bool) {
        super.setSelected(selected, animated: animated)

        // Configure the view for the selected state
    }

}
