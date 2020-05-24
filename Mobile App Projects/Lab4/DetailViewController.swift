

import UIKit

class DetailViewController: UIViewController {
    
    var selectedCity:String?
    
    
    @IBOutlet weak var ctLbl: UILabel!
    
    @IBOutlet weak var ctImage: UIImageView!
    
    
    @IBOutlet weak var desc: UITextView!
    
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        if selectedCity == "London"
        {
            self.ctLbl.text = selectedCity
            self.ctImage.image = UIImage(named: "London.jpg")
            self.desc.text = "London, the capital of England and the United Kingdom, is a 21st-century city with history stretching back to Roman times. At its centre stand the imposing Houses of Parliament, the iconic ‘Big Ben’ clock tower and Westminster Abbey, site of British monarch coronations. Across the Thames River, the London Eye observation wheel provides panoramic views of the South Bank cultural complex, and the entire city. (Wikipedia)"
        }
        else if selectedCity == "Paris"
        {
            self.ctLbl.text = selectedCity
            self.ctImage.image = UIImage(named: "Paris.jpg")
            self.desc.text = "Paris, France's capital, is a major European city and a global center for art, fashion, gastronomy and culture. Its 19th-century cityscape is crisscrossed by wide boulevards and the River Seine. Beyond such landmarks as the Eiffel Tower and the 12th-century, Gothic Notre-Dame cathedral, the city is known for its cafe culture and designer boutiques along the Rue du Faubourg Saint-Honoré. (Wikipedia)"
            
        }
        else if selectedCity == "Sydney"
        {
            self.ctLbl.text = selectedCity
            self.ctImage.image = UIImage(named: "Sydney.jpg")
            self.desc.text = "Sydney, capital of New South Wales and one of Australia's largest cities, is best known for its harbourfront Sydney Opera House, with a distinctive sail-like design. Massive Darling Harbour and the smaller Circular Quay port are hubs of waterside life, with the arched Harbour Bridge and esteemed Royal Botanic Garden nearby. Sydney Tower’s outdoor platform, the Skywalk, offers 360-degree views of the city and suburbs. (Wikipedia)"
            
        }
        else if selectedCity == "Tokyo"
        {
            self.ctLbl.text = selectedCity
            self.ctImage.image = UIImage(named: "Tokyo.jpg")
            self.desc.text = "Tokyo, Japan’s busy capital, mixes the ultramodern and the traditional, from neon-lit skyscrapers to historic temples. The opulent Meiji Shinto Shrine is known for its towering gate and surrounding woods. The Imperial Palace sits amid large public gardens. The city's many museums offer exhibits ranging from classical art (in the Tokyo National Museum) to a reconstructed kabuki theater (in the Edo-Tokyo Museum). (Wikipedia)"
            
        }
        else if selectedCity == "Cairo"
        {
            self.ctLbl.text = selectedCity
            self.ctImage.image = UIImage(named: "Cairo.jpg")
            self.desc.text = "Cairo, Egypt’s sprawling capital, is set on the Nile River. At its heart is Tahrir Square and the vast Egyptian Museum, a trove of antiquities including royal mummies and gilded King Tutankhamun artifacts. Nearby, Giza is the site of the iconic pyramids and Great Sphinx, dating to the 26th century BC. In Gezira Island’s leafy Zamalek district, 187m Cairo Tower affords panoramic city views. (Wikipedia)"
            
        }
        else
        {
            let scheme = "https"
             let host = "www.google.com"
             let path = "/search"
             let queryItem = URLQueryItem(name: "q", value: selectedCity)
             
             
             var urlComponents = URLComponents()
             urlComponents.scheme = scheme
             urlComponents.host = host
             urlComponents.path = path
             urlComponents.queryItems = [queryItem]
             
             UIApplication.shared.openURL(urlComponents.url!)
        }
        
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    

    /*
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        // Get the new view controller using segue.destinationViewController.
        // Pass the selected object to the new view controller.
    }
    */

}
