
import os, sqlite3
from scrapy import Spider
from scrapy.http import Request


class ShoesSpider(Spider):
    name = "shoes"
    allowed_domains = ["store.nike.com", "dickssportinggoods.com", "final-score.com",
                        "soccergarage.com", "u90soccer.com"]

    start_urls = ["http://store.nike.com/us/en_us/pw/mens-clearance-soccer-shoes/47Z7puZ896Zoi3",
                  "http://www.dickssportinggoods.com/products/clearance-soccer-cleats.jsp",
                  "http://www.final-score.com/Mens/Soccer/Shoes/_-_/N-1pZ1e0Zne/keyword-clearance?cm_REF=Soccer&Nr=AND%28P_RecordType%3AProduct%29",
                  "http://www.soccergarage.com/Discount-Soccer-Cleats.html",
                  "https://u90soccer.com/indoor-soccer-shoes.html",
                  "https://u90soccer.com/firm-ground-soccer-cleats.html",
                  "https://u90soccer.com/turf-soccer-shoes.html"]


    def parse(self, response):

        if "store.nike.com" in response.url:
            shoes = response.xpath('//*[@class="grid-item-image-wrapper sprite-sheet sprite-index-0"]/a/@href').extract()
            for shoe in shoes:
                yield Request(shoe, callback=self.parse_shoes)

        elif "dickssportinggoods.com" in response.url:
            shoes = response.xpath('//*[@class="fplpTitle header4"]/a/@href').extract()
            for shoe in shoes:
                yield Request(shoe, callback=self.parse_shoes)

        elif "final-score.com" in response.url:
            shoes = response.xpath('//*[@id="endeca_search_results"]/ul/li/a/@href').extract()
            for shoe in shoes:
                yield Request(shoe, callback=self.parse_shoes)

            # process next page
            next_page_url = response.xpath('.//*[@class="next"]/@href').extract_first()
            absolute_next_page_url = response.urljoin(next_page_url)
            yield Request(absolute_next_page_url)

        elif "soccergarage.com" in response.url:
            shoes = response.xpath('.//*[@class="pr_list_title"]/a/@href').extract()
            for shoe in shoes:
                yield Request(shoe, callback=self.parse_shoes)

            # process next page
            next_page_url = response.xpath('.//*[@class="page_item next_page"]/div/a/@href').extract_first()
            absolute_next_page_url = response.urljoin(next_page_url)
            yield Request(absolute_next_page_url)

        elif "u90soccer.com" in response.url:
            shoes = response.xpath('.//h2/a/@href').extract()
            for shoe in shoes:
                yield Request(shoe, callback=self.parse_shoes)

            # process next page
            next_page_url = response.xpath('.//*[@class="next button-move"]/a/@href').extract_first()
            if next_page_url is not None:
                yield Request(next_page_url)
            else:
                pass

    def parse_shoes(self, response):
        url = response.url

        if "store.nike.com" in url:
            name = response.xpath('//*[@itemprop="name"]/text()').extract_first()
            price = response.xpath('//*[@itemprop="price"]/text()').extract_first()

            shoe_type = response.css('.exp-product-subtitle::text').extract_first()
            sizes = response.xpath('//*[@class="nsg-form--drop-down exp-pdp-size-dropdown exp-pdp-dropdown two-column-dropdown"]/option')
            sizes = sizes.xpath('text()[not(parent::option/@class="exp-pdp-size-not-in-stock selectBox-disabled")]').extract()
            sizes = [s.strip() for s in sizes]

        elif "dickssportinggoods.com" in url:
            sizes = response.xpath('//*[@class="swatches clearfix"]/input/@value').extract()
            if sizes == []:
                return
            name = response.xpath('.//*[@id="PageHeading_3074457345618261107"]/h1/text()').extract_first()
            price = response.xpath('.//*[@itemprop="price"]/text()').extract_first()
            shoe_type =  ''

        elif "final-score.com" in url:
            sizes = response.xpath('.//*[@class="available"]/text()').extract()
            if sizes == []:
                return

            name = response.xpath('.//*[@itemprop="name"]/text()').extract_first()
            price = response.xpath('.//*[@itemprop="price"]/text()').extract_first()

            shoe_type = ''

        elif "soccergarage.com" in response.url:
            sizes = response.xpath('.//*[@class="property-value"]/select/option/text()').extract()
            sizes = [i.strip() for i in sizes]
            del sizes[0]
            del sizes[-1]
            name = response.xpath('.//h1/text()').extract_first()
            price = response.xpath('.//*[@id="product_price"]/text()').extract_first()
            shoe_type = ''

        elif "u90soccer.com" in url:
            sizes = response.xpath('.//*[@id="attsize"]/ul/li/a/span/text()').extract()
            name = response.xpath('.//*[@class="product-name"]/h1/text()').extract_first()
            price = response.xpath('.//*[@class="price"]/text()').extract_first()
            price = price.strip()
            if 'indoor' in url:
                shoe_type = 'Soccer Indoor Shoes'
            elif 'turf' in url:
                shoe_type = 'Soccer Turf Shoes'
            else:
                shoe_type = 'Soccer Cleats'

        sizes = [i.strip('0') for i in sizes]
        sizes = ', '.join(sizes)
        price = float(price.replace('$',''))

        conn = sqlite3.connect('shoes.db')
        c = conn.cursor()
        c.execute("INSERT INTO shoes(url,name,price,sizes,shoe_type) values(?,?,?,?,?)", (url,name,price,sizes,shoe_type))
        conn.commit()
        c.close()

        yield {
            'url': url,
            'name': name,
            'price': price,
            'sizes': sizes,
            'shoe_type': shoe_type
        }